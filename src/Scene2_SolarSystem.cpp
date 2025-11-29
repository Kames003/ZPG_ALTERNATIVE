#include "Scene2_SolarSystem.h"
#include "PointLight.h"
#include "LeafRotate.h"
#include "LeafTranslate.h"
#include "LeafScale.h"
#include <cmath>

// Indexy textúr
#define TEX_SUN 0
#define TEX_MERCURY 1
#define TEX_VENUS 2
#define TEX_EARTH 3
#define TEX_MOON 4
#define TEX_MARS 5
#define TEX_JUPITER 6
#define TEX_SATURN 7
#define TEX_URANUS 8
#define TEX_NEPTUNE 9

// Indexy objektov
#define OBJ_SUN 0
#define OBJ_MERCURY 1
#define OBJ_VENUS 2
#define OBJ_EARTH 3
#define OBJ_MOON 4
#define OBJ_MARS 5
#define OBJ_PHOBOS 6
#define OBJ_JUPITER 7
#define OBJ_SATURN 8
#define OBJ_URANUS 9
#define OBJ_NEPTUNE 10

void Scene2_SolarSystem::createShaders()
{
    // Shader s textúrou a materiálom
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");
}

void Scene2_SolarSystem::createDrawableObjects()
{
    planetModel = new LoadedModel("models/planet.obj");
    ShaderProgram* shader = spm->getShaderProgram(0);

    // ========================================
    // TEXTÚRY
    // ========================================
    tm->addTexture(new Texture2D("models/sun.jpg"));      // 0
    tm->addTexture(new Texture2D("models/merkur.jpg"));   // 1
    tm->addTexture(new Texture2D("models/venus.jpg"));    // 2
    tm->addTexture(new Texture2D("models/earth.jpg"));    // 3
    tm->addTexture(new Texture2D("models/moon.jpg"));     // 4
    tm->addTexture(new Texture2D("models/mars.jpg"));     // 5
    tm->addTexture(new Texture2D("models/jupiter.jpg"));  // 6
    tm->addTexture(new Texture2D("models/saturn.jpg"));   // 7
    tm->addTexture(new Texture2D("models/uranus.jpg"));   // 8
    tm->addTexture(new Texture2D("models/neptune .jpg")); // 9 (pozor na medzeru!)

    // ========================================
    // MATERIÁLY
    // ========================================
    // Emissive pre Slnko - žiari samo
    Material* sunMat = new Material(
        glm::vec3(1.0f, 1.0f, 1.0f),   // ambient - plná intenzita
        glm::vec3(0.0f, 0.0f, 0.0f),   // diffuse - žiadne
        glm::vec3(0.0f, 0.0f, 0.0f),   // specular - žiadne
        1.0f, "Sun_Emissive"
    );

    // Planéty - Phong osvetlenie
    Material* planetMat = new Material(
        glm::vec3(0.15f, 0.15f, 0.15f),  // ambient
        glm::vec3(1.0f, 1.0f, 1.0f),     // diffuse
        glm::vec3(0.3f, 0.3f, 0.3f),     // specular
        32.0f, "Planet"
    );

    // Mesiace - matnejšie
    Material* moonMat = new Material(
        glm::vec3(0.12f, 0.12f, 0.12f),
        glm::vec3(0.9f, 0.9f, 0.9f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        16.0f, "Moon"
    );

    // Plynné obry - mierne lesklé (atmosféra)
    Material* gasGiantMat = new Material(
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        64.0f, "GasGiant"
    );

    // ========================================
    // SLNKO
    // ========================================
    DrawableObject* sun = new DrawableObject(planetModel, shader);
    sun->addTexture(tm->getTexture(TEX_SUN));
    sun->setMaterial(sunMat);
    sun->scale(glm::vec3(SUN_SIZE));
    sun->calculateModelMatrix();
    sun->updateModelMatrix();
    om->addDrawableObject(sun);

    // ========================================
    // MERKÚR - najbližšie k Slnku
    // ========================================
    DrawableObject* mercury = new DrawableObject(planetModel, shader);
    mercury->addTexture(tm->getTexture(TEX_MERCURY));
    mercury->setMaterial(planetMat);
    mercury->scale(glm::vec3(MERCURY_SIZE));
    mercury->calculateModelMatrix();
    mercury->updateModelMatrix();
    om->addDrawableObject(mercury);

    // ========================================
    // VENUŠA
    // ========================================
    DrawableObject* venus = new DrawableObject(planetModel, shader);
    venus->addTexture(tm->getTexture(TEX_VENUS));
    venus->setMaterial(planetMat);
    venus->scale(glm::vec3(VENUS_SIZE));
    venus->calculateModelMatrix();
    venus->updateModelMatrix();
    om->addDrawableObject(venus);

    // ========================================
    // ZEM
    // ========================================
    DrawableObject* earth = new DrawableObject(planetModel, shader);
    earth->addTexture(tm->getTexture(TEX_EARTH));
    earth->setMaterial(planetMat);
    earth->scale(glm::vec3(EARTH_SIZE));
    earth->calculateModelMatrix();
    earth->updateModelMatrix();
    om->addDrawableObject(earth);

    // ========================================
    // MESIAC - obieha okolo Zeme
    // ========================================
    DrawableObject* moon = new DrawableObject(planetModel, shader);
    moon->addTexture(tm->getTexture(TEX_MOON));
    moon->setMaterial(moonMat);
    moon->scale(glm::vec3(MOON_SIZE));
    moon->calculateModelMatrix();
    moon->updateModelMatrix();
    om->addDrawableObject(moon);

    // ========================================
    // MARS
    // ========================================
    DrawableObject* mars = new DrawableObject(planetModel, shader);
    mars->addTexture(tm->getTexture(TEX_MARS));
    mars->setMaterial(planetMat);
    mars->scale(glm::vec3(MARS_SIZE));
    mars->calculateModelMatrix();
    mars->updateModelMatrix();
    om->addDrawableObject(mars);

    // ========================================
    // PHOBOS - obieha okolo Marsu
    // ========================================
    DrawableObject* phobos = new DrawableObject(planetModel, shader);
    phobos->addTexture(tm->getTexture(TEX_MOON));
    phobos->setMaterial(moonMat);
    phobos->scale(glm::vec3(PHOBOS_SIZE));
    phobos->calculateModelMatrix();
    phobos->updateModelMatrix();
    om->addDrawableObject(phobos);

    // ========================================
    // JUPITER - najväčšia planéta
    // ========================================
    DrawableObject* jupiter = new DrawableObject(planetModel, shader);
    jupiter->addTexture(tm->getTexture(TEX_JUPITER));
    jupiter->setMaterial(gasGiantMat);
    jupiter->scale(glm::vec3(JUPITER_SIZE));
    jupiter->calculateModelMatrix();
    jupiter->updateModelMatrix();
    om->addDrawableObject(jupiter);

    // ========================================
    // SATURN - s prstencami (prstence nevytvárame)
    // ========================================
    DrawableObject* saturn = new DrawableObject(planetModel, shader);
    saturn->addTexture(tm->getTexture(TEX_SATURN));
    saturn->setMaterial(gasGiantMat);
    saturn->scale(glm::vec3(SATURN_SIZE));
    saturn->calculateModelMatrix();
    saturn->updateModelMatrix();
    om->addDrawableObject(saturn);

    // ========================================
    // URÁN - naklonený na bok (97.8°)
    // ========================================
    DrawableObject* uranus = new DrawableObject(planetModel, shader);
    uranus->addTexture(tm->getTexture(TEX_URANUS));
    uranus->setMaterial(gasGiantMat);
    uranus->scale(glm::vec3(URANUS_SIZE));
    uranus->calculateModelMatrix();
    uranus->updateModelMatrix();
    om->addDrawableObject(uranus);

    // ========================================
    // NEPTÚN - najďalej
    // ========================================
    DrawableObject* neptune = new DrawableObject(planetModel, shader);
    neptune->addTexture(tm->getTexture(TEX_NEPTUNE));
    neptune->setMaterial(gasGiantMat);
    neptune->scale(glm::vec3(NEPTUNE_SIZE));
    neptune->calculateModelMatrix();
    neptune->updateModelMatrix();
    om->addDrawableObject(neptune);

    printf("\n");
    printf("========================================\n");
    printf("  SOLAR SYSTEM - Composite Pattern\n");
    printf("========================================\n");
    printf("Slnko + 8 planet + 2 mesiace\n");
    printf("Realisticke pomery rychlosti obehu\n");
    printf("Rotacia okolo vlastnej osi\n");
    printf("Hierarchicke transformacie (mesiace)\n");
    printf("========================================\n\n");
}

void Scene2_SolarSystem::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene2_SolarSystem::createScene(GLFWwindow* window)
{
    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();
    this->tm = new TextureManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // Svetlo v strede (Slnko)
    PointLight* sunLight = new PointLight(
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f, 0.0f, 0.01f,  // Nízka attenuation - dosahuje ďaleko
        glm::vec3(1.0f, 0.95f, 0.85f)
    );
    lm->addPointLight(sunLight);
    lm->ambient = 0.05f;
    spm->updateLights();

    // Kamera ďalej pre celý pohľad
    camera->setPosition(glm::vec3(0.0f, 12.0f, 15.0f));
    camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Scene2_SolarSystem::renderFrame()
{
    camera->checkChanges();
    camera->controls();

    float time = (float)glfwGetTime();

    // ========================================
    // SLNKO - v strede, len rotácia okolo vlastnej osi
    // Poradie aplikácie: scale → rotate (pridávame v opačnom poradí)
    // ========================================
    DrawableObject* sun = om->getDrawableObject(OBJ_SUN);
    sun->getTransformationComposite()->removeTransformations();
    sunAxis = fmod(time * SUN_AXIS_SPEED * SPEED_MULT, 360.0f);
    sun->rotate(sunAxis, glm::vec3(0.0f, 1.0f, 0.0f));  // 2. rotácia okolo osi
    sun->scale(glm::vec3(SUN_SIZE));                     // 1. scale
    sun->calculateModelMatrix();
    sun->updateModelMatrix();

    // ========================================
    // MERKÚR - bez náklonu
    // Poradie aplikácie: scale → axis_rot → translate → orbit_rot
    // ========================================
    DrawableObject* mercury = om->getDrawableObject(OBJ_MERCURY);
    mercury->getTransformationComposite()->removeTransformations();
    mercuryOrbit = fmod(time * MERCURY_ORBIT_SPEED * SPEED_MULT, 360.0f);
    mercuryAxis = fmod(time * MERCURY_AXIS_SPEED * SPEED_MULT, 360.0f);
    // Pridávame v OPAČNOM poradí (posledné pridané = prvé aplikované)
    mercury->rotate(mercuryOrbit, glm::vec3(0.0f, 1.0f, 0.0f));  // 4. orbit okolo Slnka
    mercury->translate(glm::vec3(MERCURY_DIST, 0.0f, 0.0f));      // 3. posun od Slnka
    mercury->rotate(mercuryAxis, glm::vec3(0.0f, 1.0f, 0.0f));    // 2. rotácia okolo osi
    mercury->scale(glm::vec3(MERCURY_SIZE));                       // 1. škálovanie
    mercury->calculateModelMatrix();
    mercury->updateModelMatrix();

    // ========================================
    // VENUŠA - retrográdna rotácia (záporná rýchlosť)
    // ========================================
    DrawableObject* venus = om->getDrawableObject(OBJ_VENUS);
    venus->getTransformationComposite()->removeTransformations();
    venusOrbit = fmod(time * VENUS_ORBIT_SPEED * SPEED_MULT, 360.0f);
    venusAxis = fmod(time * VENUS_AXIS_SPEED * SPEED_MULT, 360.0f);
    venus->rotate(venusOrbit, glm::vec3(0.0f, 1.0f, 0.0f));
    venus->translate(glm::vec3(VENUS_DIST, 0.0f, 0.0f));
    venus->rotate(venusAxis, glm::vec3(0.0f, 1.0f, 0.0f));
    venus->scale(glm::vec3(VENUS_SIZE));
    venus->calculateModelMatrix();
    venus->updateModelMatrix();

    // ========================================
    // ZEM - s náklonom osi 23.5°
    // Poradie aplikácie: scale → axis_rot → tilt → translate → orbit
    // (pridávame v opačnom poradí kvôli násobeniu matíc)
    // ========================================
    DrawableObject* earth = om->getDrawableObject(OBJ_EARTH);
    earth->getTransformationComposite()->removeTransformations();
    earthOrbit = fmod(time * EARTH_ORBIT_SPEED * SPEED_MULT, 360.0f);
    earthAxis = fmod(time * EARTH_AXIS_SPEED * SPEED_MULT, 360.0f);
    earth->rotate(earthOrbit, glm::vec3(0.0f, 1.0f, 0.0f));       // 5. orbit okolo Slnka
    earth->translate(glm::vec3(EARTH_DIST, 0.0f, 0.0f));           // 4. posun od Slnka
    earth->rotate(EARTH_TILT, glm::vec3(0.0f, 0.0f, 1.0f));        // 3. náklon osi
    earth->rotate(earthAxis, glm::vec3(0.0f, 1.0f, 0.0f));         // 2. rotácia okolo osi (deň/noc)
    earth->scale(glm::vec3(EARTH_SIZE));                            // 1. scale
    earth->calculateModelMatrix();
    earth->updateModelMatrix();

    // ========================================
    // MESIAC - hierarchicky závisí na Zemi
    // Poradie: scale → moon_translate → moon_orbit → earth_translate → earth_orbit
    // ========================================
    DrawableObject* moon = om->getDrawableObject(OBJ_MOON);
    moon->getTransformationComposite()->removeTransformations();
    moonOrbit = fmod(time * MOON_ORBIT_SPEED * SPEED_MULT, 360.0f);
    moon->rotate(earthOrbit, glm::vec3(0.0f, 1.0f, 0.0f));         // 5. Zem okolo Slnka
    moon->translate(glm::vec3(EARTH_DIST, 0.0f, 0.0f));             // 4. posun na orbit Zeme
    moon->rotate(moonOrbit, glm::vec3(0.0f, 1.0f, 0.0f));           // 3. Mesiac okolo Zeme (ORIGINAL - horizontalna)
    //moon->rotate(moonOrbit, glm::vec3(0.0f, 0.0f, 1.0f));           // 3. Mesiac okolo Zeme (TEST - cez vrch/spodok, Z os)
    moon->translate(glm::vec3(MOON_DIST, 0.0f, 0.0f));              // 2. posun od Zeme
    moon->scale(glm::vec3(MOON_SIZE));                               // 1. scale
    moon->calculateModelMatrix();
    moon->updateModelMatrix();

    // ========================================
    // MARS - s náklonom osi 25.2°
    // Poradie aplikácie: scale → axis_rot → tilt → translate → orbit
    // ========================================
    DrawableObject* mars = om->getDrawableObject(OBJ_MARS);
    mars->getTransformationComposite()->removeTransformations();
    marsOrbit = fmod(time * MARS_ORBIT_SPEED * SPEED_MULT, 360.0f);
    marsAxis = fmod(time * MARS_AXIS_SPEED * SPEED_MULT, 360.0f);
    mars->rotate(marsOrbit, glm::vec3(0.0f, 1.0f, 0.0f));           // 5. orbit
    mars->translate(glm::vec3(MARS_DIST, 0.0f, 0.0f));               // 4. translate
    mars->rotate(MARS_TILT, glm::vec3(0.0f, 0.0f, 1.0f));            // 3. tilt
    mars->rotate(marsAxis, glm::vec3(0.0f, 1.0f, 0.0f));             // 2. axis rotation
    mars->scale(glm::vec3(MARS_SIZE));                                // 1. scale
    mars->calculateModelMatrix();
    mars->updateModelMatrix();

    // ========================================
    // PHOBOS - hierarchicky závisí na Marse
    // ========================================
    DrawableObject* phobos = om->getDrawableObject(OBJ_PHOBOS);
    phobos->getTransformationComposite()->removeTransformations();
    phobosOrbit = fmod(time * PHOBOS_ORBIT_SPEED * SPEED_MULT, 360.0f);
    phobos->rotate(marsOrbit, glm::vec3(0.0f, 1.0f, 0.0f));
    phobos->translate(glm::vec3(MARS_DIST, 0.0f, 0.0f));
    phobos->rotate(phobosOrbit, glm::vec3(0.0f, 1.0f, 0.0f));
    phobos->translate(glm::vec3(PHOBOS_DIST, 0.0f, 0.0f));
    phobos->scale(glm::vec3(PHOBOS_SIZE));
    phobos->calculateModelMatrix();
    phobos->updateModelMatrix();

    // ========================================
    // JUPITER - najrýchlejšia rotácia okolo osi
    // ========================================
    DrawableObject* jupiter = om->getDrawableObject(OBJ_JUPITER);
    jupiter->getTransformationComposite()->removeTransformations();
    jupiterOrbit = fmod(time * JUPITER_ORBIT_SPEED * SPEED_MULT, 360.0f);
    jupiterAxis = fmod(time * JUPITER_AXIS_SPEED * SPEED_MULT, 360.0f);
    jupiter->rotate(jupiterOrbit, glm::vec3(0.0f, 1.0f, 0.0f));
    jupiter->translate(glm::vec3(JUPITER_DIST, 0.0f, 0.0f));
    jupiter->rotate(jupiterAxis, glm::vec3(0.0f, 1.0f, 0.0f));
    jupiter->scale(glm::vec3(JUPITER_SIZE));
    jupiter->calculateModelMatrix();
    jupiter->updateModelMatrix();

    // ========================================
    // SATURN - s náklonom 26.7°
    // Poradie aplikácie: scale → axis_rot → tilt → translate → orbit
    // ========================================
    DrawableObject* saturn = om->getDrawableObject(OBJ_SATURN);
    saturn->getTransformationComposite()->removeTransformations();
    saturnOrbit = fmod(time * SATURN_ORBIT_SPEED * SPEED_MULT, 360.0f);
    saturnAxis = fmod(time * SATURN_AXIS_SPEED * SPEED_MULT, 360.0f);
    saturn->rotate(saturnOrbit, glm::vec3(0.0f, 1.0f, 0.0f));        // 5. orbit
    saturn->translate(glm::vec3(SATURN_DIST, 0.0f, 0.0f));            // 4. translate
    saturn->rotate(SATURN_TILT, glm::vec3(0.0f, 0.0f, 1.0f));         // 3. tilt
    saturn->rotate(saturnAxis, glm::vec3(0.0f, 1.0f, 0.0f));          // 2. axis rotation
    saturn->scale(glm::vec3(SATURN_SIZE));                             // 1. scale
    saturn->calculateModelMatrix();
    saturn->updateModelMatrix();

    // ========================================
    // URÁN - extrémny náklon 97.8° (rotuje "na boku")
    // Poradie aplikácie: scale → axis_rot → tilt → translate → orbit
    // ========================================
    DrawableObject* uranus = om->getDrawableObject(OBJ_URANUS);
    uranus->getTransformationComposite()->removeTransformations();
    uranusOrbit = fmod(time * URANUS_ORBIT_SPEED * SPEED_MULT, 360.0f);
    uranusAxis = fmod(time * URANUS_AXIS_SPEED * SPEED_MULT, 360.0f);
    uranus->rotate(uranusOrbit, glm::vec3(0.0f, 1.0f, 0.0f));        // 5. orbit
    uranus->translate(glm::vec3(URANUS_DIST, 0.0f, 0.0f));            // 4. translate
    uranus->rotate(URANUS_TILT, glm::vec3(0.0f, 0.0f, 1.0f));         // 3. tilt (97.8° - takmer na boku!)
    uranus->rotate(uranusAxis, glm::vec3(0.0f, 1.0f, 0.0f));          // 2. axis rotation
    uranus->scale(glm::vec3(URANUS_SIZE));                             // 1. scale
    uranus->calculateModelMatrix();
    uranus->updateModelMatrix();

    // ========================================
    // NEPTÚN - najďalej od Slnka
    // Poradie aplikácie: scale → axis_rot → tilt → translate → orbit
    // ========================================
    DrawableObject* neptune = om->getDrawableObject(OBJ_NEPTUNE);
    neptune->getTransformationComposite()->removeTransformations();
    neptuneOrbit = fmod(time * NEPTUNE_ORBIT_SPEED * SPEED_MULT, 360.0f);
    neptuneAxis = fmod(time * NEPTUNE_AXIS_SPEED * SPEED_MULT, 360.0f);
    neptune->rotate(neptuneOrbit, glm::vec3(0.0f, 1.0f, 0.0f));      // 5. orbit
    neptune->translate(glm::vec3(NEPTUNE_DIST, 0.0f, 0.0f));          // 4. translate
    neptune->rotate(NEPTUNE_TILT, glm::vec3(0.0f, 0.0f, 1.0f));       // 3. tilt
    neptune->rotate(neptuneAxis, glm::vec3(0.0f, 1.0f, 0.0f));        // 2. axis rotation
    neptune->scale(glm::vec3(NEPTUNE_SIZE));                           // 1. scale
    neptune->calculateModelMatrix();
    neptune->updateModelMatrix();

    spm->updateLights();
    om->drawObjects();
}

void Scene2_SolarSystem::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.02f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderFrame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}