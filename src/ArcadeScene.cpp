#include "ArcadeScene.h"
#include "PlainModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Callback.h"
#include "Texture2D.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Texture indices (poradie podla pridavania v kode)
#define TEX_SHREK 0
#define TEX_FIONA 1
#define TEX_TOILET 2
#define TEX_CAT 3
#define TEX_HAMMER 4
#define TEX_DESK 5   // desk.jpg sa pridava prvy v createPedestals
#define TEX_MAP 6    // mapa.jpg sa pridava druhy

ArcadeScene::ArcadeScene()
    : score(0), hits(0), misses(0), gameTime(0.0f), maxGameTime(60.0f),
      gameOver(false), gamePaused(false),
      shrekModel(nullptr), fionaModel(nullptr), toiletModel(nullptr), catModel(nullptr),
      hammerModel(nullptr), hammer(nullptr),
      spawnTimer(0.0f), spawnInterval(1.5f), maxActiveMoles(3),
      keyRWasPressed(false)
{
    for (int i = 0; i < NUM_PEDESTALS; i++)
    {
        pedestals[i] = nullptr;
        pedestalOccupied[i] = false;
    }
}

ArcadeScene::~ArcadeScene()
{
    for (Mole* m : moles)
    {
        delete m;
    }
    moles.clear();

    if (hammer) delete hammer;
}

void ArcadeScene::createScene(GLFWwindow* window)
{
    printf("\n");
    printf("========================================\n");
    printf("     WHAC-A-MOLE - 3D ARCADE GAME\n");
    printf("========================================\n\n");

    this->window = window;

    // Nahodne seedovanie
    srand(static_cast<unsigned>(time(nullptr)));

    // Vytvor kameru - FIXNY POHLAD ZHORA
    this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);

    // Managery
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();
    this->tm = new TextureManager();

    createShaders();
    createLights();
    loadCharacterModels();
    createPedestals();
    callbacks();

    // Nastav fixnu kameru - pohlad zhora na podstavce (vyssia pre vacsie hrisko)
    camera->setPosition(glm::vec3(0.0f, 15.0f, 10.0f));
    camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    printf("\n");
    printf("========================================\n");
    printf("    GAME READY - WHACK THE MOLES!\n");
    printf("========================================\n");
    printf("Controls:\n");
    printf("  MIDDLE CLICK - Whack character\n");
    printf("  R            - Restart game\n");
    printf("  ESC          - Exit\n");
    printf("========================================\n");
    printf("Characters:\n");
    printf("  SHREK   = +10 pts (common)\n");
    printf("  FIONA   = +25 pts (medium)\n");
    printf("  TOILET  = +50 pts (rare!)\n");
    printf("========================================\n\n");
}

void ArcadeScene::createShaders()
{
    printf("Creating shaders...\n");

    // Phong shader s texturami (index 0)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTexture.glsl");

    // Phong shader bez textury (index 1) - pre podstavce
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    printf("Shaders created: PhongTexture (0), Phong (1)\n");
}

void ArcadeScene::createDrawableObjects()
{
    // Objekty sa vytvaraju v createPedestals() a spawnMole()
}

void ArcadeScene::createLights()
{
    printf("Creating lights...\n");

    // Silnejsie ambient svetlo
    lm->ambient = 0.5f;

    // Hlavne svetlo zhora
    DirectionalLight* mainLight = new DirectionalLight(
        glm::normalize(glm::vec3(0.0f, -1.0f, 0.3f)),
        glm::vec3(0.8f, 0.8f, 0.8f)
    );
    lm->addDirectionalLight(mainLight);

    // Bodove svetlo nad hraco plochou
    PointLight* topLight = new PointLight(
        glm::vec3(0.0f, 8.0f, 0.0f),
        0.8f, 0.09f, 0.032f,
        glm::vec3(1.0f, 0.95f, 0.9f)
    );
    lm->addPointLight(topLight);

    spm->updateLights();
    printf("Lights created\n");
}

void ArcadeScene::loadCharacterModels()
{
    printf("Loading character models...\n");

    // Shrek (index 0)
    shrekModel = new LoadedModel("models/shrek.obj");
    tm->addTexture(new Texture2D("models/shrek.png"));
    printf("  Shrek loaded\n");

    // Fiona (index 1)
    fionaModel = new LoadedModel("models/fiona.obj");
    tm->addTexture(new Texture2D("models/fiona.png"));
    printf("  Fiona loaded\n");

    // Toilet (WC) (index 2)
    toiletModel = new LoadedModel("models/toiled.obj");
    tm->addTexture(new Texture2D("models/toiled.jpg"));
    printf("  Toilet loaded\n");

    // Cat (index 3)
    catModel = new LoadedModel("models/Cat.obj");
    tm->addTexture(new Texture2D("models/Cat_diffuse.jpg"));
    printf("  Cat loaded\n");

    // Hammer (index 4)
    hammerModel = new LoadedModel("models/kladivo.obj");
    tm->addTexture(new Texture2D("models/kladivo.jpg"));
    printf("  Hammer loaded\n");

    // Vytvor hammer controller
    ShaderProgram* textureShader = spm->getShaderProgram(0);
    hammer = new Hammer(hammerModel, textureShader);
    hammer->setScale(0.08f);  // Velkost kladiva - mensie
    hammer->setVisibleDuration(0.4f);  // Ako dlho zostane viditelne po udere

    // Pridaj texturu a do object managera
    Texture* hammerTex = tm->getTexture(TEX_HAMMER);
    if (hammerTex)
    {
        hammer->getVisual()->addTexture(hammerTex);
    }
    om->addDrawableObject(hammer->getVisual());

    printf("All character models loaded!\n");
}

void ArcadeScene::createPedestals()
{
    printf("Creating desk with map...\n");

    ShaderProgram* textureShader = spm->getShaderProgram(0);  // Shader s texturou

    // === STOL (desk) - pod mapou ===
    LoadedModel* deskModel = new LoadedModel("models/desk.obj");
    tm->addTexture(new Texture2D("models/desk.jpg"));  // TEX_DESK = 5

    DrawableObject* desk = new DrawableObject(deskModel, textureShader);
    desk->addTexture(tm->getTexture(TEX_DESK));
    desk->rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));  // Uplne naležato - vidime vrch
    desk->scale(glm::vec3(0.08f, 0.08f, 0.08f));  // Vacsie
    desk->translate(glm::vec3(0.0f, -2.0f, 0.0f));  // Pod mapou
    desk->calculateModelMatrix();
    desk->updateModelMatrix();
    om->addDrawableObject(desk);
    printf("  Desk loaded\n");

    // === MAPA - horizontalne na urovni Y=0 ===
    LoadedModel* mapModel = new LoadedModel("models/mapa.obj");
    tm->addTexture(new Texture2D("models/mapa.jpg"));  // TEX_MAP = 6

    DrawableObject* mapFloor = new DrawableObject(mapModel, textureShader);
    mapFloor->addTexture(tm->getTexture(TEX_MAP));
    mapFloor->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));   // Poloz horizontalne
    mapFloor->rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));  // Otoc
    mapFloor->scale(glm::vec3(0.4f, 0.4f, 0.4f));  // Mensie aby sa zmestila na stol
    mapFloor->translate(glm::vec3(0.0f, 1.0f, 16.0f));  // Povodna pozicia
    mapFloor->calculateModelMatrix();
    mapFloor->updateModelMatrix();
    om->addDrawableObject(mapFloor);
    printf("  Map loaded (horizontal at Y=0)\n");

    // Pozicie kde postavy vyskakuju - MENSIE SPACING pre mapu na stole
    float spacingX = 1.25f;  // Znizene z 4.0
    float spacingZ = 1.25f;  // Znizene z 4.0
    float startX = -spacingX;
    float startZ = -spacingZ / 1.25f;

    int index = 0;
    for (int row = 0; row < PEDESTAL_ROWS; row++)
    {
        for (int col = 0; col < PEDESTAL_COLS; col++)
        {
            float x = startX + col * spacingX;
            float z = startZ + row * spacingZ;

            // Uloz poziciu (pre spawn logiku) - ale NEVYTVARAJ vizualne diery
            pedestalPositions[index] = glm::vec3(x, 6.5f, z + 1.0f);  // Na mape - viditelna vyska
            pedestals[index] = nullptr;  // Ziadne vizualne pedestaly

            printf("  Spawn point %d at (%.1f, 0, %.1f)\n", index, x, z);
            index++;
        }
    }

    printf("Map floor created - characters will walk on it!\n");
}

Mole* ArcadeScene::spawnMole()
{
    // Najdi DVA volne podstavce - start a ciel
    int startPedestal = getFreePedestal();
    if (startPedestal < 0)
    {
        return nullptr;  // Vsetky obsadene
    }
    pedestalOccupied[startPedestal] = true;  // Docasne obsad

    // Najdi cielovy podstavec (rozny od startovacieh)
    int endPedestal = getFreePedestal();
    if (endPedestal < 0)
    {
        // Len jeden volny - pouzi staticku poziciu
        endPedestal = startPedestal;
    }

    // Nahodny typ postavy
    Mole::Type type = getRandomMoleType();

    // === OBJECT POOLING: Skus najst existujuci neaktivny mole rovnakeho typu ===
    Mole* mole = nullptr;
    for (Mole* m : moles)
    {
        if (!m->isActive() && m->getType() == type)
        {
            mole = m;  // Znovupouzi existujuci!
            break;
        }
    }

    // Ak sme nenasli, vytvor novy
    if (mole == nullptr)
    {
        AbstractModel* model = nullptr;
        int textureIndex = 0;

        switch (type)
        {
            case Mole::Type::SHREK:
                model = shrekModel;
                textureIndex = TEX_SHREK;
                break;
            case Mole::Type::FIONA:
                model = fionaModel;
                textureIndex = TEX_FIONA;
                break;
            case Mole::Type::TOILET:
                model = toiletModel;
                textureIndex = TEX_TOILET;
                break;
            case Mole::Type::CAT:
                model = catModel;
                textureIndex = TEX_CAT;
                break;
        }

        ShaderProgram* textureShader = spm->getShaderProgram(0);
        mole = new Mole(model, textureShader, type);

        // Nastav texturu
        Texture* texture = tm->getTexture(textureIndex);
        if (texture)
        {
            mole->getVisual()->addTexture(texture);
        }

        // Pridaj vizual do object managera (len pre NOVE moles)
        om->addDrawableObject(mole->getVisual());
        mole->setStencilID(om->getObjectCount());

        moles.push_back(mole);
        printf("[POOL] Created NEW %s (total: %zu)\n", Mole::getNameForType(type), moles.size());
    }
    else
    {
        printf("[POOL] Reused existing %s\n", Mole::getNameForType(type));
    }

    // Spawn s trajektoriou medzi podstavcami
    glm::vec3 startPos = pedestalPositions[startPedestal];
    glm::vec3 endPos = pedestalPositions[endPedestal];

    if (startPedestal != endPedestal)
    {
        // Pohyb medzi dvoma podstavcami - rezervuj OBA podstavce
        pedestalOccupied[endPedestal] = true;
        mole->spawnWithPath(startPos, endPos, startPedestal, endPedestal);
    }
    else
    {
        // Staticka pozicia (len jeden volny podstavec)
        mole->spawn(startPos, startPedestal);
    }

    return mole;
}

Mole::Type ArcadeScene::getRandomMoleType()
{
    int roll = rand() % 100;

    if (roll < 14)  // 14% sanca - WC
    {
        return Mole::Type::TOILET;
    }
    else if (roll < 39)  // 25% sanca - Fiona
    {
        return Mole::Type::FIONA;
    }
    else  // 65% sanca - Shrek
    {
        return Mole::Type::SHREK;
    }
}

int ArcadeScene::getFreePedestal()
{
    // Najdi volne podstavce
    std::vector<int> free;
    for (int i = 0; i < NUM_PEDESTALS; i++)
    {
        if (!pedestalOccupied[i])
        {
            free.push_back(i);
        }
    }

    if (free.empty())
    {
        return -1;
    }

    // Nahodny vyber
    return free[rand() % free.size()];
}

void ArcadeScene::updateMoles(float deltaTime)
{
    // Spawn novych postav
    spawnTimer += deltaTime;

    // DYNAMICKA OBTIAZNOST - rychlejsie spawny casom
    // Start: 1.5s interval, po 30s: 0.8s interval
    float difficultyFactor = glm::clamp(gameTime / 30.0f, 0.0f, 1.0f);
    float currentSpawnInterval = glm::mix(1.5f, 0.8f, difficultyFactor);

    // Zvys max aktivnych molov casom (3 -> 5)
    int currentMaxMoles = 3 + static_cast<int>(difficultyFactor * 2);

    if (spawnTimer >= currentSpawnInterval)
    {
        spawnTimer = 0.0f;

        // Spocitaj aktivne
        int activeCount = 0;
        for (Mole* m : moles)
        {
            if (m->isActive()) activeCount++;
        }

        // Spawn ak je miesto
        if (activeCount < currentMaxMoles)
        {
            spawnMole();
        }
    }

    // Update existujucich
    for (Mole* m : moles)
    {
        if (m->isActive())
        {
            int startPedestal = m->getPedestalIndex();
            int targetPedestal = m->getTargetPedestalIndex();

            m->update(deltaTime);

            // Ak uz nie je aktivny, uvolni OBA podstavce
            if (!m->isActive())
            {
                if (startPedestal >= 0 && startPedestal < NUM_PEDESTALS)
                {
                    pedestalOccupied[startPedestal] = false;
                }
                if (targetPedestal >= 0 && targetPedestal < NUM_PEDESTALS && targetPedestal != startPedestal)
                {
                    pedestalOccupied[targetPedestal] = false;
                }
                misses++;
                printf("[MISSED] %s escaped! Misses: %d\n",
                       Mole::getNameForType(m->getType()), misses);
            }
        }
    }
}

void ArcadeScene::handleInput()
{
    // Restart (R)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (!keyRWasPressed)
        {
            restartGame();
            keyRWasPressed = true;
        }
    }
    else
    {
        keyRWasPressed = false;
    }
}

void ArcadeScene::handleWhacking()
{
    // Pouzivame MIDDLE CLICK na whacking (ma stencil picking)
    if (Callback::hasMiddleClick())
    {
        // Vypocitaj 3D poziciu kliknutia pre kladivo
        if (hammer)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            int width, height;
            glfwGetWindowSize(window, &width, &height);

            float ndcX = (2.0f * mouseX / width) - 1.0f;
            float ndcY = 1.0f - (2.0f * mouseY / height);

            glm::mat4 proj = camera->getProjectionMatrix();
            glm::mat4 view = camera->getViewMatrix();
            glm::mat4 invVP = glm::inverse(proj * view);

            glm::vec4 rayStart = invVP * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
            glm::vec4 rayEnd = invVP * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);
            rayStart /= rayStart.w;
            rayEnd /= rayEnd.w;

            glm::vec3 rayDir = glm::normalize(glm::vec3(rayEnd - rayStart));
            glm::vec3 rayOrigin = glm::vec3(rayStart);

            float planeY = 6.5f;  // Viditelna vyska mapy
            if (rayDir.y != 0.0f)
            {
                float t = (planeY - rayOrigin.y) / rayDir.y;
                glm::vec3 hitPoint = rayOrigin + t * rayDir;
                hitPoint.x = glm::clamp(hitPoint.x, -6.0f, 6.0f);
                hitPoint.z = glm::clamp(hitPoint.z, -6.0f, 6.0f);  // Vacsie limity pre mapu

                hammer->strike(hitPoint);
            }
        }

        int clickedID = Callback::getClickedObjectID();
        printf("[CLICK] Stencil ID: %d\n", clickedID);

        bool hitSomething = false;

        // Hladaj mole s danym ID
        for (Mole* m : moles)
        {
            if (m->isActive() && m->getStencilID() == clickedID)
            {
                int startPedestal = m->getPedestalIndex();
                int targetPedestal = m->getTargetPedestalIndex();
                int points = m->hit();

                if (points != 0)
                {
                    score += points;
                    hits++;

                    // Uvolni OBA podstavce (start aj ciel)
                    if (startPedestal >= 0 && startPedestal < NUM_PEDESTALS)
                    {
                        pedestalOccupied[startPedestal] = false;
                    }
                    if (targetPedestal >= 0 && targetPedestal < NUM_PEDESTALS && targetPedestal != startPedestal)
                    {
                        pedestalOccupied[targetPedestal] = false;
                    }

                    if (points > 0)
                    {
                        printf("[WHACKED!] +%d pts! Score: %d\n", points, score);
                    }
                    else
                    {
                        printf("[OOPS! Hit the cat!] %d pts! Score: %d\n", points, score);
                    }
                }

                hitSomething = true;
                break;
            }
        }

        if (!hitSomething && clickedID > NUM_PEDESTALS + 1)
        {
            printf("[MISS] Nothing there!\n");
        }

        Callback::clearClicks();
    }
}

void ArcadeScene::checkGameOver()
{
    if (gameTime >= maxGameTime && !gameOver)
    {
        gameOver = true;
        printf("\n");
        printf("========================================\n");
        printf("           TIME'S UP!\n");
        printf("========================================\n");
        printf("Final Score: %d points\n", score);
        printf("Hits: %d\n", hits);
        printf("Misses: %d\n", misses);
        printf("Accuracy: %.1f%%\n", hits > 0 ? (float)hits / (hits + misses) * 100.0f : 0.0f);
        printf("========================================\n");
        if (score >= 300) printf("   WHAC-A-MOLE MASTER!\n");
        else if (score >= 200) printf("   SKILLED WHACKER!\n");
        else if (score >= 100) printf("   AMATEUR WHACKER\n");
        else printf("   NEEDS MORE PRACTICE...\n");
        printf("========================================\n");
        printf("Press R to play again\n");
        printf("========================================\n\n");
    }
}

void ArcadeScene::restartGame()
{
    printf("\n[NEW GAME] Starting new round...\n\n");

    // Reset skore
    score = 0;
    hits = 0;
    misses = 0;
    gameTime = 0.0f;
    gameOver = false;
    spawnTimer = 0.0f;

    // Skry vsetky moles a uvolni podstavce
    for (Mole* m : moles)
    {
        m->hide();
    }

    for (int i = 0; i < NUM_PEDESTALS; i++)
    {
        pedestalOccupied[i] = false;
    }

    // Novy seed
    srand(static_cast<unsigned>(time(nullptr)));
}

void ArcadeScene::displayHUD()
{
    // Konzolovy HUD (kazdu sekundu)
    static float hudTimer = 0.0f;
    hudTimer += 0.016f;

    if (hudTimer >= 1.0f)
    {
        hudTimer = 0.0f;

        if (!gameOver)
        {
            printf("Time: %.0f | Score: %d | Hits: %d | Misses: %d\n",
                   maxGameTime - gameTime, score, hits, misses);
        }
    }
}

void ArcadeScene::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        Callback::GetInstance().mouseButtonCallback(w, button, action, mods);
    });

    printf("Callbacks registered\n");
}

void ArcadeScene::renderFrame()
{
    // Delta time
    static float lastTime = 0.0f;
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (deltaTime > 0.1f) deltaTime = 0.1f;

    // Kamera - checkChanges ale nie controls (fixna kamera)
    camera->checkChanges();

    // Aktualizuj kladivo (timer pre skrytie)
    if (hammer)
    {
        hammer->update(deltaTime);
    }

    // Input
    handleInput();

    // Hra
    if (!gameOver)
    {
        gameTime += deltaTime;
        updateMoles(deltaTime);
        handleWhacking();
        checkGameOver();
    }
    else
    {
        handleWhacking();  // Clear clicks
    }

    // HUD
    displayHUD();

    // Svetla
    spm->updateLights();

    // Renderovanie
    om->drawObjects();
}

void ArcadeScene::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // Tmave pozadie

    // Stencil buffer pre picking
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    printf("\n[GAME START] Whack those moles!\n\n");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        renderFrame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
