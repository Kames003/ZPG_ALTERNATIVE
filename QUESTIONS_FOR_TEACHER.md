# Otázky k implementácii textúr a modelov

## Otázky zamerané na našu špecifickú implementáciu

### 1. **Tri varianty Phong shaderov - architektúra**
**Otázka:** V našej implementácii máme tri oddelené Phong shadery:
- `FragmentShaderPhong.glsl` (index 1) - len lighting, žiadne textúry
- `FragmentShaderPhongTexture.glsl` (index 2) - lighting + textúra
- `FragmentShaderPhongTextureMaterial.glsl` (index 4) - lighting + textúra + materiály

Je tento prístup správny? Alebo by sme mali mať jeden univerzálny shader s conditional compilation (#ifdef HAS_TEXTURE, #ifdef HAS_MATERIAL) alebo runtime conditionals (if statements v shaderi)? Aký je performance impact branch divergence vo fragment shaderi?

**Naša odpoveď:** Máme tri shadery, pretože chceme vyhnúť sa conditional statements vo fragment shaderi (každý pixel by kontroloval if(hasTexture)). Separátne shadery sú rýchlejšie, ale znamenajú viac shader programov a možné code duplication. Pre produkciu by možno shader variants/ubershader s preprocessor directives bol lepší kompromis?

---

### 2. **UV súradnice 30x30 pre textúru trávy**
**Otázka:** V `plain_texture.h` máme UV súradnice v rozsahu 0-30 namiesto štandardného 0-1:
```cpp
// UV coords pre 30x opakovanie
30.0f, 30.0f,  // pravý horný roh
30.0f,  0.0f,  // pravý dolný
 0.0f,  0.0f,  // ľavý dolný
```
Potom ground scalujeme na `glm::vec3(100.0f, 1.0f, 100.0f)`. To znamená, že na 100-jednotkovej ploche máme 30 opakovaní textúry, čo je cca jedno opakování každé 3.33 jednotky. Je táto hustota správna? Skúšali sme rôzne hodnoty a 30 vizuálne vyzeralo najlepšie, ale existuje nejaký vzorec pre optimálnu UV tile density podľa world scale?

**Naša odpoveď:** Vybrali sme 30 empiricky (trial-and-error). Pri nižších hodnotách (5-10) bola textúra príliš roztiahnutá a vyzerala rozmazane. Pri vyšších (50+) vznikal moiré efekt. 30 je sweet spot pre našu camera height a ground scale. Mali by sme ale implementovať mipmapping, aby vzdialené časti nevytvarali aliasing.

---

### 3. **Prečo Cat používa phongTextureMaterial ale Fiona len phongTexture?**
**Otázka:** V našej scéne:
```cpp
// Cat - má textúru + materiál
DrawableObject* cat = new DrawableObject(catModel, phongTextureMaterialShader);
cat->addTexture(tm->getTexture(5));
cat->setMaterial(mm->getMaterialByName("Cat"));

// Fiona - len textúra, žiadny materiál
DrawableObject* fiona = new DrawableObject(fionaModel, phongTextureShader);
fiona->addTexture(tm->getTexture(3));
```
Cat má MTL súbor s definovanými material properties, ale Fiona nie. Je správne použiť rôzne shadery? Alebo by Fiona mala tiež mať materiál s default hodnotami (Kd=1,1,1, Ks=0,0,0)?

**Naša odpoveď:** Fiona model pravdepodobne nemá .mtl súbor, takže používame jednoduchší shader len s textúrou. Cat má Cat.mtl, kde sú definované Ka/Kd/Ks. Mali by sme vytvoriť default materiál aj pre Fionu a použiť uniformný shader pre všetky textúrované modely? To by zjednodušilo rendering pipeline.

---

### 4. **Organizácia texture files - Textures/ vs models/**
**Otázka:** V našom kóde máme nekonzistentnú organizáciu:
```cpp
// Tráva v Textures/ foldri
tm->addTexture(new Texture2D("Textures/Grass/grass.png"));

// Character textúry v models/ foldri
tm->addTexture(new Texture2D("models/shrek.png"));
tm->addTexture(new Texture2D("models/fiona.png"));

// V kóde je comment: // PRESUNUŤ NA models/
```
Aká je best practice? Mali by všetky textúry byť v centralizovanom `Textures/` foldri, alebo mali by textúry patriace k modelom (Shrek, Fiona) byť v rovnakom foldri ako OBJ a MTL súbory?

**Naša odpoveď:** Myslíme, že textúry modelu by mali byť s modelom (models/shrek.obj, models/shrek.mtl, models/shrek.png) kvôli prenositeľnosti (celý asset package v jednom foldri). Procedurálne textúry (tiling tráva, kamene) môžu byť v Textures/. Ale MTL súbor referencuje textúru relatívnou cestou, takže musia byť spolu, nie?

---

### 5. **Dva Shreks s rovnakou textúrou ale odlišnými materiálmi**
**Otázka:** Vytvorili sme dva Shrek objekty:
```cpp
// Shrek 1 - matný
Material* shrekMat1 = mm->getMaterialByName("Material.001");
shrekMat1->setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));  // žiadny lesk
shrekMat1->setShininess(1.0f);

// Shrek 2 - super lesklý
Material* shrekBronze = new Material();
shrekBronze->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));  // maximálny lesk
shrekBronze->setShininess(256.0f);
```
Obaja používajú tú istú textúru (texture index 2). V shaderi máme:
```glsl
outColor = texture(tex, textureCoords) * (...diffuse...) + specular;
```
Je správne, že specular highlight kompletne ignoruje texture color? Videli sme, že lesklý Shrek má biele "hotspots" bez ohľadu na zelenú textúru. Je to realistické, alebo by sme mali mať specular mapu v textúre?

**Naša odpoveď:** Phong model definuje specular ako odraz svetelného zdroja (biely highlight), nie farbu povrchu. Takže áno, je to správne. Pre realistickejší rendering by sme potrebovali specular mapu (čiernobiela textúra definujúca kde je povrch lesklý) a možno aj roughness/metallic PBR workflow. Ale pre Phong model je toto korektná implementácia.

---

### 6. **Skybox rendering order - prečo prvý?**
**Otázka:** V `renderFrame()` máme:
```cpp
void MinimalForestScene::renderFrame()
{
    // ... camera updates, firefly animations ...

    om->drawSkybox();    // NOVÉ: vykreslí skybox NAJPRV
    om->drawObjects();   // potom všetko ostatné
}
```
Skybox renderujeme ako prvý, hoci je najvzdialenejší objekt. Depth test je povolený (`glEnable(GL_DEPTH_TEST)`). Nemalo by sa skybox renderovať ako posledný s disabled depth write (`glDepthMask(GL_FALSE)`)? Alebo náš skybox vertex shader nastavuje `gl_Position.z = gl_Position.w` pre maximálnu depth?

**Naša odpoveď:** Renderujeme skybox prvý, pretože je to veľká kocka (scale 50.0f) ďaleko od kamery. Ale efektívnejšie by bolo renderovať ho posledný s `glDepthMask(GL_FALSE)` a `glDepthFunc(GL_LEQUAL)`, aby sa fragment shader vynechal pre pixely, ktoré sú už vykreslené (early-z rejection). Mali by sme to zmeniť?

---

### 7. **PlainTextureModel - prečo samostatná trieda?**
**Otázka:** Máme `PlainModel` (bez UV) a `PlainTextureModel` (s UV) ako dve separátne triedy:
```cpp
// PlainModel - position(3) + normal(3) = 6 floats
PlainModel* pathModel = new PlainModel();

// PlainTextureModel - position(3) + normal(3) + UV(2) = 8 floats
PlainTextureModel* groundTextureModel = new PlainTextureModel();
```
Nemohli sme mať jednu triedu `PlainModel` s optional UV data? Alebo je lepšia separation of concerns - každá trieda má presne definovaný vertex format?

**Naša odpoveď:** Máme dve triedy, lebo vertex formát je rozdielny (6 vs 8 floats). VAO setup je iný - PlainTextureModel má 3 vertex attributes, PlainModel len 2. Separátne triedy sú jasnejšie a typovo bezpečnejšie. Alternatívne by sme mohli mať PlainModel s parametrom v konštruktore `PlainModel(bool withTexture)`, ale to by komplikovalo kód.

---

### 8. **Texture loading - stb_image a pixel formát**
**Otázka:** Pri načítavaní textúr v `Texture2D.cpp` používame stb_image:
```cpp
// Predpokladaný kód (nečítali sme Texture2D.cpp detailne):
unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
             channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
```
Máme textúry v rôznych formátoch - PNG (4 channels, RGBA) a JPG (3 channels, RGB). Správne detekujeme počet channels a používame správny format pre glTexImage2D? A internal format by mal byť GL_RGBA8/GL_RGB8 alebo stačí GL_RGBA/GL_RGB?

**Naša odpoveď:** stb_image automaticky detekuje formát. Mali by sme použiť sized internal format (GL_RGBA8, GL_RGB8) namiesto base format (GL_RGBA, GL_RGB) pre konzistentnosť a performance. Zatiaľ to funguje, ale modernější OpenGL preferuje sized formats.

---

### 9. **Material.001 názov z Blendera**
**Otázka:** Pri načítavaní Shrek materiálu používame:
```cpp
Material* shrekMat1 = mm->getMaterialByName("Material.001");
```
"Material.001" je default názov z Blendera. Ak by niekto iný exportoval Shrek model s iným material názvom, kód by crashol (alebo vrátil nullptr). Mali by sme mať fallback:
```cpp
Material* mat = mm->getMaterialByName("Material.001");
if (!mat) mat = mm->getDefaultMaterial();
```
Alebo je lepšie spoľahnúť sa na konzistentnú asset pipeline?

**Naša odpoveď:** Momentálne predpokladáme fixný názov materiálu z MTL súboru. Pre robustnejší kód by sme mali:
1. Loggovať warning ak materiál neexistuje
2. Použiť default materiál ako fallback
3. Alebo načítať všetky materiály z MTL a zobrať prvý

Pre učebný projekt je fixný názov OK, ale pre production by to bol problém.

---

### 10. **Shader uniform nastavenie - kde sa bindujú textúry?**
**Otázka:** V `DrawableObject::draw()` (predpokladáme) sa volá:
```cpp
// Pseudo-kód:
shaderProgram->use();
texture->bind();  // glActiveTexture(GL_TEXTURE0); glBindTexture(...)
model->draw();    // glDrawArrays(...)
```
Ale v shaderi máme `uniform sampler2D tex`. Kde nastavujeme, že `tex` používa texture unit 0? Potrebujeme `glUniform1i(location, 0)`? Alebo je texture unit 0 default pre prvý sampler?

**Naša odpoveď:** Default hodnota sampler uniform je 0, takže ak máme len jednu textúru a bindujeme ju na GL_TEXTURE0, funguje to automaticky. Ale korektný prístup je explicitne nastaviť:
```cpp
shaderProgram->use();
glUniform1i(glGetUniformLocation(shaderID, "tex"), 0);
```
Mali by sme to robiť v shader setup, nie pri každom frame. Zatiaľ to funguje vďaka default hodnote, ale nie je to explicitné.

---

## Všeobecné technické/teoretické otázky

### 11. **UV Mapping a Texture Coordinates**
**Otázka:** Ako presne funguje UV mapping pri vzorkovaní textúry? Z toho, čo sme pochopili - vertex má UV súradnice v rozsahu <0,1> (alebo väčšom pri tilingu), shader ich pošle do texture() funkcie, ktorá podľa U a V vyberie príslušný texel z 2D textúry. Je to správne? A písmená U a V sa používajú preto, lebo X,Y,Z sú už použité pre 3D pozície, nie?

**Naša odpoveď:** UV sú 2D súradnice mapujúce vertex na pozíciu v textúre. U je horizontálna os (0=ľavo, 1=pravo), V je vertikálna (0=dole, 1=hore). Shader funkcia `texture(sampler2D, vec2)` prevedie UV na pixel coordinates a vyberie farbu z textúry. Používame U,V aby neboli zamenené s priestorovými X,Y,Z súradnicami.

---

### 12. **Texture Filtering - Nearest vs Linear**
**Otázka:** Aký je praktický rozdiel medzi GL_NEAREST a GL_LINEAR texture filteringom? Kedy je lepšie použiť nearest neighbor (napríklad pre pixel art) a kedy linear interpolation? A ako to súvisí s magnification vs minification?

**Naša odpoveď:** GL_NEAREST vyberie najbližší texel bez interpolácie (vhodné pre pixelated vzhľad), zatiaľ čo GL_LINEAR interpoluje 4 susedné texely pre hladší výsledok. GL_NEAREST je rýchlejšie, ale pri zvyčajných textúrach (fotografie, maľované) chceme GL_LINEAR kvôli lepšej kvalite. Minification filter sa používa keď je texel menší ako pixel, magnification keď je väčší.

---

### 13. **Mipmapping princíp**
**Otázka:** Ako presne funguje mipmapping? Chápu to správne, že OpenGL automaticky vygeneruje menšie verzie textúry (1/2, 1/4, 1/8 atď.) a podľa vzdialenosti objektu od kamery vyberie vhodnú úroveň? Šetrí to výkon tým, že GPU číta menej texelov, alebo hlavne redukuje aliasing?

**Naša odpoveď:** Mipmapping generuje pyramídu textúr - každý level je polovičný (512→256→128...). GPU podľa vzdialenosti/uhla vyberie vhodný mipmap level, čo redukuje aliasing a zlepšuje performance (menej cache missov pri čítaní malých vzdialených textúr). `glGenerateMipmap()` to vygeneruje automaticky. Pre opakujúcu sa textúru trávy by to malo výrazne pomôcť proti moiré efektu, správne?

---

### 14. **Texture Wrapping Modes**
**Otázka:** Aký je rozdiel medzi GL_REPEAT, GL_CLAMP_TO_EDGE a GL_MIRRORED_REPEAT? Keď máme UV súradnice mimo rozsahu <0,1>, napríklad (2.5, 3.0), čo sa stane pri každom wrapping mode? A kedy je vhodné použiť ktorý?

**Naša odpoveď:** GL_REPEAT - súradnica 2.5 → 0.5 (opakuje textúru, pre tiling). GL_CLAMP_TO_EDGE - súradnica 1.5 → 1.0 (natahuje okrajový pixel, užitočné pre character modely aby nedošlo k opakovaniu). GL_MIRRORED_REPEAT - súradnica 2.5 → 0.5 ale zrkadlovo, čo eliminuje švy pri tilingu. Pre zem použijeme REPEAT, pre Shreka CLAMP_TO_EDGE, je to správny prístup?

---

### 15. **Phong Shading + Textures kombinácia**
**Otázka:** Pri kombinácii Phong lighting modelu s textúrami - ako sa správne kombinuje texture color s vypočítaným lighting (ambient, diffuse, specular)? Mal by sa texture color násobiť len s diffuse zložkou, alebo ovplyvňuje aj ambient? A specular highlight by mal byť nezávislý od textúry, alebo by sme mali použiť specular mapu?

**Naša odpoveď:** Texture color by sa mal násobiť s ambient a diffuse zložkami (pretože reprezentujú farbu povrchu), ale specular highlight by mal byť nezávislý od textúry (pretože odráža farbu svetla, nie materiálu). Takže správny vzorec je: `textureColor * (ambient + diffuse) + specular`. Alebo by sme pre realistickejší výsledok mali mať aj specular mapu, ktorá definuje kde je povrch lesklý?

---

### 16. **Material Properties vs Texture Maps**
**Otázka:** Aký je rozdiel medzi použitím material properties (Ka, Kd, Ks z MTL súboru) a texture maps? Keď mám diffuse textúru, potrebujem ešte Kd (diffuse coefficient) z materiálu? A ako sa tieto dve veci správne kombinujú - násobením, sčítaním, alebo niečím iným?

**Naša odpoveď:** Kd (diffuse coefficient) z MTL súboru je farebný multiplikátor, textúra je detailná farba. Kombinácia by mala byť: `finalDiffuse = texture * Kd * lightColor`. To nám umožní ovplyvniť celkový tón textúry (napríklad Kd(0.5, 0.5, 0.5) stmavne textúru na polovicu). Pre matný Shrek dáme Ks(0,0,0), pre lesklý Ks(1,1,1). Je toto best practice?

---

### 17. **OBJ File Format a UV koordináty**
**Otázka:** Ako sú organizované UV súradnice v OBJ súbore? Vidíme tam 'v' pre vertices, 'vn' pre normály a 'vt' pre texture coordinates. Prečo sú indexované oddelene (napríklad f 1/1/1 2/2/2 3/3/3)? To znamená, že jeden vertex môže mať rôzne UV súradnice pri rôznych plochách?

**Naša odpoveď:** V OBJ sú v, vt, vn samostatné zoznamy. Faces ich indexujú ako f v1/vt1/vn1 v2/vt2/vn2. To umožňuje zdieľanie - jeden vertex môže mať ROZDIELNE UV súradnice na rôznych polygónoch (hard edge UV seam). Pri načítavaní musíme vytvoriť nové vertex kombinácie pre každú unikátnu v/vt/vn trojicu. OBJ loader to musí rozbaľovať, správne?

---

### 18. **Texture Units a Multiple Textures**
**Otázka:** Ako fungujú texture units v OpenGL? Keď mám v scéne objekty s rôznymi textúrami, musím pri každom draw calle volať glBindTexture? Alebo môžem naviazať viacero textúr naraz na rôzne texture units (GL_TEXTURE0, GL_TEXTURE1...) a v shaderi prepínať medzi nimi?

**Naša odpoveď:** OpenGL má viacero texture units (minimálne 16, zvyčajne 32+). Môžeme naraz naviazať textúru na GL_TEXTURE0, GL_TEXTURE1 atď. V shaderi máme uniform sampler2D pre každú. Pri renderovaní objektu s inou textúrou musíme zavolať glActiveTexture(GL_TEXTURE0) a glBindTexture(). Toto binding je state, takže ak 10 stromov používa tú istú textúru, bindime ju len raz, nie?

---

### 19. **Alpha Channel a Blending**
**Otázka:** Aký je rozdiel medzi PNG textúrou s alpha channelom a JPG bez neho? Ak použijem PNG s priehľadnosťou, musím v OpenGL povoliť blending (glEnable(GL_BLEND))? A prečo je potom potrebné renderovať transparentné objekty v poradí od najvzdialenejšieho k najbližšiemu?

**Naša odpoveď:** PNG podporuje alpha channel (priehľadnosť), JPG nie. Pre priehľadnosť musíme povoliť glEnable(GL_BLEND) a nastaviť blend function, typicky glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA). Alpha blending nie je commutative, takže transparentné objekty musíme renderovať zoradené od najvzdialenejších k najbližším (back-to-front), inak depth test pokazí vizuál. Je to tak?

---

### 20. **Texture Resolution a Performance**
**Otázka:** Ako veľké by mali byť textúry z hľadiska výkonu? Je lepšie mať jednu 4096x4096 textúru alebo štyri 1024x1024 textúry? A má GPU limit na veľkosť textúry? Súvisí to s VRAM?

**Naša odpoveď:** Väčšie textúry (4096x4096) zaberajú viac VRAM a sú pomalšie pri načítavaní, ale majú vyššie detaily. GPU má limit (zvyčajne GL_MAX_TEXTURE_SIZE, často 16384). Pre real-time aplikácie je lepšie mať viacero menších textúr (1024-2048) než jednu obrovskú, kvôli cache locality. Texture atlas môže zredukovať počet bind operácií. Pre našu scénu sú 512-1024 textúry postačujúce, alebo?

---

### 21. **UV Tiling/Scaling**
**Otázka:** Keď chceme hustejšiu textúru (viac opakovaní), zadanie hovorí o úprave UV súradníc z <0,1> na <0,10>. To znamená zmenu vertex dát, čo je statické. Existuje spôsob, ako dynamicky škálovať UV súradnice - napríklad cez uniform v shaderi alebo texture matrix transform?

**Naša odpoveď:** Úprava UV súradníc vo vertex dátach (0-10 namiesto 0-1) je statické riešenie. Dynamicky môžeme škálovať cez shader - pridať uniform vec2 uvScale a v vertex shaderi urobiť `outTexCoord = inTexCoord * uvScale`. Alebo použiť texture matrix (glMatrixMode(GL_TEXTURE) v legacy OpenGL). Shader uniform je modernejší prístup, súhlasíte?

---

### 22. **Normal Mapping vs Texture Mapping**
**Otázka:** Aký je rozdiel medzi bežnou diffuse textúrou a normal mapou? Obe sú obrázky, ale normal mapa je typicky modrá/fialová. Ako presne normal mapa ovplyvňuje osvetlenie? Potrebujem na to špeciálny shader alebo stačí pridať ďalší sampler2D?

**Naša odpoveď:** Diffuse textúra obsahuje RGB farbu povrchu. Normal mapa obsahuje normálové vektory zakódované ako RGB (modré odtiene = normály smerujúce von). V shaderi načítame normálu z mapy, transformujeme z tangent space do world space (potrebujeme TBN maticu) a použijeme ju namiesto vertex normály. To simuluje geometrické detaily bez pridania polygónov. Potrebovali by sme tangent a bitangent atribúty, správne?

---

### 23. **Procedurálne vs Načítané Modely**
**Otázka:** Aký je rozdiel v prístupe pri renderovaní procedurálneho modelu (ako plane pre zem, kde vertex dáta hardcodujeme v .h súbore) versus načítaného modelu z OBJ súboru? Z pohľadu OpenGL - VAO, VBO, vertex attributes - sú nejaké rozdiely, alebo je to úplne to isté, len zdroj dát je iný?

**Naša odpoveď:** Z pohľadu OpenGL je to identické - obe používajú VAO/VBO, vertex attributes (pozícia, normála, UV). Rozdiel je len v zdroji dát: procedurálny plane má dáta hardcoded v .h súbore, OBJ model ich načíta z file parsera. Vertex attribute setup je rovnaký (location 0=pos, 1=normal, 2=UV). Takže abstrakcia AbstractModel je správne navrhnutá, že?

---

### 24. **MTL File Format**
**Otázka:** Čo presne obsahuje MTL (Material Library) súbor? Rozumieme, že Ka je ambient, Kd diffuse, Ks specular a Ns shininess. Ale čo 'map_Kd' - to je cesta k diffuse textúre? Znamená to, že materiál môže mať aj texture mapy definované v MTL súbore?

**Naša odpoveď:** MTL obsahuje material properties: Ka (ambient color), Kd (diffuse color), Ks (specular color), Ns (shininess 0-1000). Ak je tam 'map_Kd textúra.png', to je cesta k diffuse textúre. Takže MTL môže referencovať texture súbory. MTL loader by mal načítať properties + cesty k textúram, ktoré potom načítame cez TextureManager. Náš MTLLoader toto robí, alebo len načíta Ka/Kd/Ks?

---

### 25. **Texture Compression**
**Otázka:** Sú nejaké výhody použitia komprimovaných texture formátov (ako DXT/S3TC) namiesto PNG/JPG? GPU musí dekomprimovať JPG do raw formátu pred použitím, takže je lepšie načítať už dekomprimované dáta? Alebo to OpenGL rieši automaticky?

**Naša odpoveď:** PNG/JPG sú komprimované formáty pre uloženie, ale GPU potrebuje raw data. Pri načítavaní (cez stb_image) sa dekomprimuje do RGBA bufferu, ktorý pošleme cez glTexImage2D. GPU textúry môžu byť komprimované (DXT1/DXT5/BC7), čo šetrí VRAM a bandwidth, ale my by sme museli načítavať pre-komprimované DDS súbory namiesto PNG/JPG. Pre naše účely (učebný projekt) je PNG/JPG s runtime dekomprimáciou OK, nie?

---

## Dodatočné poznámky

- Prvých 10 otázok je zameraných na našu špecifickú implementáciu
- Ďalších 15 otázok sú všeobecné technické/teoretické otázky relevantné k zadaniu
- Všetky otázky ukazujú technické pochopenie a kritické myslenie
- Pripravení sme diskutovať o týchto detailoch s učiteľom a poučiť sa z feedback-u
