#include "MinimalForestScene.h"
#include "TreeModel.h"
#include "PlainModel.h"
#include "PlainTextureModel.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotlightCamera.h"
#include "BenchModel.h"
#include "BushModel.h"
#include "SphereModel.h"
#include "Firefly.h"
#include "SkyboxModel.h"
#include <cstdlib>
#include "LoadedModel.h"
#include <ctime>
#include "MaterialManager.h"
#include "MTLLoader.h"
#include "Material.h"

void MinimalForestScene::createShaders()
{
    printf("  Creating shaders...\n");

    // Skybox (index 0)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderSkybox.glsl",
        "Shaders/FragmentShaderSkybox.glsl");

    // Phong (index 1)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    // Phong Texture (index 2)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTexture.glsl");

    // Constant for fireflies (index 3)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderConstant.glsl",
        "Shaders/FragmentShaderConstant.glsl");

    // Phong Texture + Material (index 4)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");

    // Phong Material without texture (index 5)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhongMaterial.glsl");

    // W-TEST shader w=20 with texture (index 6)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTextureWTest.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");

    // W-TEST shader without texture (index 7)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongWTest.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    // W-TEST shader w=500 with texture (index 8)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTextureWTest.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");

    printf("Shaders 0-8 created\n");
}

void MinimalForestScene::createTextures()
{
    printf("  Creating textures...\n");

    // Cubemap for skybox (index 0)
    tm->addTexture(new TextureCubemap(
        "Textures/Skybox/posx.jpg",
        "Textures/Skybox/negx.jpg",
        "Textures/Skybox/posy.jpg",
        "Textures/Skybox/negy.jpg",
        "Textures/Skybox/posz.jpg",
        "Textures/Skybox/negz.jpg"
    ));

    tm->addTexture(new Texture2D("Textures/Grass/grass.png"));  // index 1
    tm->addTexture(new Texture2D("models/shrek.png"));          // index 2
    tm->addTexture(new Texture2D("models/fiona.png"));          // index 3
    tm->addTexture(new Texture2D("models/toiled.jpg"));         // index 4
    tm->addTexture(new Texture2D("models/Cat_diffuse.jpg"));    // index 5

    printf("Textures 0-5 loaded\n");
}

void MinimalForestScene::createMaterials()
{
    printf("  Creating materials...\n");
    MTLLoader::loadMTL("models/shrek.mtl", mm);
    MTLLoader::loadMTL("models/Cat.mtl", mm);
    printf("Materials loaded\n");
}

void MinimalForestScene::createLights()
{
    printf("  Creating lights...\n");

    DirectionalLight* moonLight = new DirectionalLight(
        glm::normalize(glm::vec3(0.3f, -1.0f, 0.2f)),
        glm::vec3(0.08f, 0.08f, 0.12f)
    );

    lm->addDirectionalLight(moonLight);
    lm->ambient = 0.10f;

    flashlight = new SpotlightCamera(
        1.0f, 0.09f, 0.032f,
        cos(glm::radians(12.5f)),
        glm::vec3(1.0f, 1.0f, 0.9f)
    );

    spm->updateLights();
    printf("Lights created (press F for flashlight)\n");
}

void MinimalForestScene::createDrawableObjects()
{
    printf("  Creating objects...\n");
    srand(static_cast<unsigned>(time(nullptr)));

    ShaderProgram* skyboxShader = spm->getShaderProgram(0);
    ShaderProgram* phongShader = spm->getShaderProgram(1);
    ShaderProgram* phongTextureShader = spm->getShaderProgram(2);
    ShaderProgram* constantShader = spm->getShaderProgram(3);
    ShaderProgram* phongTextureMaterialShader = spm->getShaderProgram(4);
    ShaderProgram* phongMaterialShader = spm->getShaderProgram(5);
    ShaderProgram* wTestTextureShader = spm->getShaderProgram(6);
    ShaderProgram* wTestShader = spm->getShaderProgram(7);
    ShaderProgram* wTestTextureShader500 = spm->getShaderProgram(8);

    // Set W-TEST uniform values
    wTestTextureShader->activateShaderProgram();
    wTestTextureShader->updateUniform("customW", 20.0f);
    wTestTextureShader->deactivateShaderProgram();

    wTestTextureShader500->activateShaderProgram();
    wTestTextureShader500->updateUniform("customW", 500.0f);
    wTestTextureShader500->deactivateShaderProgram();

    // SKYBOX
    SkyboxModel* skyboxModel = new SkyboxModel();
    DrawableObject* skybox = new DrawableObject(skyboxModel, skyboxShader);
    skybox->addTexture(tm->getTexture(0));
    skybox->scale(glm::vec3(50.0f));
    skybox->calculateModelMatrix();
    skybox->updateModelMatrix();
    om->addSkybox(skybox);

    // GROUND with grass texture
    PlainTextureModel* groundTextureModel = new PlainTextureModel();
    DrawableObject* ground = new DrawableObject(groundTextureModel, phongTextureShader);
    ground->addTexture(tm->getTexture(1));
    ground->scale(glm::vec3(100.0f, 1.0f, 100.0f));
    ground->translate(glm::vec3(0.0f, -0.5f, 0.0f));
    ground->calculateModelMatrix();
    ground->updateModelMatrix();
    om->addDrawableObject(ground);

    // PATH
    PlainModel* pathModel = new PlainModel();
    DrawableObject* path = new DrawableObject(pathModel, phongShader);
    path->setColor(glm::vec3(0.6f, 0.5f, 0.4f));
    path->scale(glm::vec3(3.0f, 1.0f, 100.0f));
    path->translate(glm::vec3(0.0f, -0.4f, 0.0f));
    path->calculateModelMatrix();
    path->updateModelMatrix();
    om->addDrawableObject(path);

    // TREES (70)
    TreeModel* treeModel = new TreeModel();
    for (int i = 0; i < 70; i++)
    {
        float x, z;
        do {
            x = (rand() % 80) - 40.0f;
            z = (rand() % 80) - 40.0f;
        } while (abs(x) < 2.5f);

        float scale = 0.2f + static_cast<float>(rand() % 15) / 100.0f;
        float rotation = static_cast<float>(rand() % 360);

        DrawableObject* tree = new DrawableObject(treeModel, phongShader);
        tree->setColor(glm::vec3(0.1f, 0.2f, 0.05f));
        tree->translate(glm::vec3(x, -0.5f, z));
        tree->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        tree->scale(glm::vec3(scale));
        tree->calculateModelMatrix();
        tree->updateModelMatrix();
        om->addDrawableObject(tree);
    }

    // BENCHES (4)
    BenchModel* benchModel = new BenchModel();
    float benchPositions[][3] = {
        {-5.0f, -0.5f, -10.0f}, {5.0f, -0.5f, -10.0f},
        {-5.0f, -0.5f, 10.0f}, {5.0f, -0.5f, 10.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        DrawableObject* bench = new DrawableObject(benchModel, phongShader);
        bench->setColor(glm::vec3(0.55f, 0.17f, 0.05f));
        bench->translate(glm::vec3(benchPositions[i][0], benchPositions[i][1], benchPositions[i][2]));
        bench->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        bench->scale(glm::vec3(1.5f));
        bench->calculateModelMatrix();
        bench->updateModelMatrix();
        om->addDrawableObject(bench);
    }

    // LAMPS (10) with point lights
    LoadedModel* lampModel = new LoadedModel("models/lamp.obj");
    float lampPositions[][3] = {
        {-2.5f, 0.32f, -20.0f}, {2.5f, 0.32f, -20.0f},
        {-2.5f, 0.32f, -10.0f}, {2.5f, 0.32f, -10.0f},
        {-2.5f, 0.32f, 0.0f}, {2.5f, 0.32f, 0.0f},
        {-2.5f, 0.32f, 10.0f}, {2.5f, 0.32f, 10.0f},
        {-2.5f, 0.32f, 20.0f}, {2.5f, 0.32f, 20.0f}
    };

    for (int i = 0; i < 10; i++)
    {
        DrawableObject* lamp = new DrawableObject(lampModel, phongShader);
        lamp->setColor(glm::vec3(0.9f, 0.8f, 0.3f));
        lamp->translate(glm::vec3(lampPositions[i][0], lampPositions[i][1], lampPositions[i][2]));
        lamp->scale(glm::vec3(0.23f));
        lamp->calculateModelMatrix();
        lamp->updateModelMatrix();
        om->addDrawableObject(lamp);

        PointLight* lampLight = new PointLight(
            glm::vec3(lampPositions[i][0], lampPositions[i][1] + 1.5f, lampPositions[i][2]),
            1.0f, 0.35f, 0.8f,
            glm::vec3(1.0f, 0.9f, 0.7f) * 0.8f
        );
        lm->addPointLight(lampLight);
    }

    // SHUTTLE (crashing spaceship)
    LoadedModel* shuttleModel = new LoadedModel("models/shuttle.obj");
    DrawableObject* shuttle = new DrawableObject(shuttleModel, phongShader);
    shuttle->setColor(glm::vec3(0.7f, 0.7f, 0.75f));
    shuttle->translate(glm::vec3(-15.0f, 25.0f, 10.0f));
    shuttle->rotate(-25.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    shuttle->rotate(15.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    shuttle->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shuttle->scale(glm::vec3(0.5f));
    shuttle->calculateModelMatrix();
    shuttle->updateModelMatrix();
    om->addDrawableObject(shuttle);

    // ========================================
    // SHREK MODELS - W-TEST DEMONSTRATION
    // ========================================
    LoadedModel* shrekModel = new LoadedModel("models/shrek.obj");

    // SHREK 1 - W-TEST with customW=20 (should look NORMAL)
    DrawableObject* shrek1 = new DrawableObject(shrekModel, wTestTextureShader);
    shrek1->addTexture(tm->getTexture(2));
    Material* shrekMat1 = mm->getMaterialByName("Material.001");
    shrekMat1->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    shrekMat1->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekMat1->setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
    shrekMat1->setShininess(1.0f);
    shrek1->setMaterial(shrekMat1);
    shrek1->translate(glm::vec3(-6.0f, -0.5f, -5.0f));
    shrek1->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek1->scale(glm::vec3(0.8f));
    shrek1->calculateModelMatrix();
    shrek1->updateModelMatrix();
    om->addDrawableObject(shrek1);
    printf("Shrek 1 - W-TEST customW=20\n");

    // SHREK 4 - W-TEST with customW=500 (should look SAME as Shrek1)
    DrawableObject* shrek4 = new DrawableObject(shrekModel, wTestTextureShader500);
    shrek4->addTexture(tm->getTexture(2));
    shrek4->setMaterial(shrekMat1);
    shrek4->translate(glm::vec3(-4.0f, -0.5f, -5.0f));
    shrek4->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek4->scale(glm::vec3(0.8f));
    shrek4->calculateModelMatrix();
    shrek4->updateModelMatrix();
    om->addDrawableObject(shrek4);
    printf("Shrek 4 - W-TEST customW=500 (same size as Shrek1)\n");

    // SHREK 3 - CUSTOM MATRIX with [3][3]=20 (uniform scale - 20x SMALLER)
    DrawableObject* shrek3 = new DrawableObject(shrekModel, phongTextureMaterialShader);
    shrek3->addTexture(tm->getTexture(2));
    Material* shrekMat3 = new Material();
    shrekMat3->setName("ShrekCustomW");
    shrekMat3->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    shrekMat3->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekMat3->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
    shrekMat3->setShininess(32.0f);
    mm->addMaterial(shrekMat3);
    shrek3->setMaterial(shrekMat3);

    // Custom identity matrix with [3][3] = 20 (acts as uniform scale 1/20)
    glm::mat4 customIdentity = glm::mat4(1.0f);
    customIdentity[3][3] = 20.0f;

    shrek3->translate(glm::vec3(-8.0f, -0.5f, -5.0f));
    shrek3->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek3->scale(glm::vec3(0.8f));
    shrek3->customMatrix(customIdentity);  // Applied FIRST to vertex
    shrek3->calculateModelMatrix();
    shrek3->updateModelMatrix();
    om->addDrawableObject(shrek3);
    printf("Shrek 3 - CUSTOM MATRIX [3][3]=20 (20x smaller)\n");

    // SHREK 2 - Super shiny material demo
    DrawableObject* shrek2 = new DrawableObject(shrekModel, phongTextureMaterialShader);
    shrek2->addTexture(tm->getTexture(2));
    Material* shrekBronze = new Material();
    shrekBronze->setName("ShrekSuperShiny");
    shrekBronze->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    shrekBronze->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekBronze->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekBronze->setShininess(256.0f);
    mm->addMaterial(shrekBronze);
    shrek2->setMaterial(shrekBronze);
    shrek2->translate(glm::vec3(10.0f, -0.5f, -8.0f));
    shrek2->rotate(-45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek2->scale(glm::vec3(0.8f));
    shrek2->calculateModelMatrix();
    shrek2->updateModelMatrix();
    om->addDrawableObject(shrek2);
    printf("Shrek 2 - Super shiny material\n");

    // FIONA
    LoadedModel* fionaModel = new LoadedModel("models/fiona.obj");
    DrawableObject* fiona = new DrawableObject(fionaModel, phongTextureShader);
    fiona->addTexture(tm->getTexture(3));
    fiona->translate(glm::vec3(6.0f, -0.5f, -5.0f));
    fiona->rotate(-30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    fiona->scale(glm::vec3(0.75f));
    fiona->calculateModelMatrix();
    fiona->updateModelMatrix();
    om->addDrawableObject(fiona);

    // TOILET
    LoadedModel* toiletModel = new LoadedModel("models/toiled.obj");
    DrawableObject* toilet = new DrawableObject(toiletModel, phongTextureShader);
    toilet->addTexture(tm->getTexture(4));
    toilet->translate(glm::vec3(-12.0f, -0.5f, -18.0f));
    toilet->rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    toilet->scale(glm::vec3(0.5f));
    toilet->calculateModelMatrix();
    toilet->updateModelMatrix();
    om->addDrawableObject(toilet);

    // CAT on bench
    LoadedModel* catModel = new LoadedModel("models/Cat.obj");
    DrawableObject* cat = new DrawableObject(catModel, phongTextureMaterialShader);
    cat->addTexture(tm->getTexture(5));
    Material* catMat = mm->getMaterialByName("Cat");
    cat->setMaterial(catMat);
    cat->translate(glm::vec3(-5.0f, -0.06f, -10.0f));
    cat->rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    cat->rotate(360.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    cat->scale(glm::vec3(0.012f));
    cat->calculateModelMatrix();
    cat->updateModelMatrix();
    om->addDrawableObject(cat);

    // BUSHES (15)
    BushModel* bushModel = new BushModel();
    for (int i = 0; i < 15; i++)
    {
        float x, z;
        do {
            x = (rand() % 60) - 30.0f;
            z = (rand() % 60) - 30.0f;
        } while (abs(x) < 3.0f);

        float scale = 0.8f + static_cast<float>(rand() % 200) / 1000.0f;
        float rotation = static_cast<float>(rand() % 360);

        DrawableObject* bush = new DrawableObject(bushModel, phongShader);
        bush->setColor(glm::vec3(0.2f, 0.35f, 0.15f));
        bush->translate(glm::vec3(x, -0.5f, z));
        bush->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        bush->scale(glm::vec3(scale));
        bush->calculateModelMatrix();
        bush->updateModelMatrix();
        om->addDrawableObject(bush);
    }

    // FIREFLIES (12)
    SphereModel* sphereModel = new SphereModel();
    for (int i = 0; i < 12; i++)
    {
        float x, z;
        do {
            x = (rand() % 50) - 25.0f;
            z = (rand() % 50) - 25.0f;
        } while (abs(x) < 4.0f);

        float y = 1.0f + (rand() % 200) / 100.0f;

        glm::vec3 color;
        int colorType = rand() % 3;
        if (colorType == 0) color = glm::vec3(1.0f, 0.9f, 0.5f);
        else if (colorType == 1) color = glm::vec3(0.7f, 1.0f, 0.6f);
        else color = glm::vec3(1.0f, 0.95f, 0.7f);

        Firefly* firefly = new Firefly(glm::vec3(x, y, z), sphereModel, constantShader, color);
        lm->addPointLight(firefly->getLight());
        om->addDrawableObject(firefly->getVisual());
        fireflies.push_back(firefly);
    }

    // ========================================
    // BEZIER CURVE TEST - Orbiting Planet
    // ========================================
    printf("Creating Bezier curve planet...\n");

    // Define Bezier spline control points (higher than shuttle at y=25)
    planetPath = new BezierSpline();
    planetPath->addControlPoint(glm::vec3(-20.0f, 32.0f, -10.0f));  // P0
    planetPath->addControlPoint(glm::vec3(20.0f, 35.0f, -10.0f));   // P1
    planetPath->addControlPoint(glm::vec3(-20.0f, 38.0f, 10.0f));   // P2
    planetPath->addControlPoint(glm::vec3(20.0f, 32.0f, 10.0f));    // P3
    planetPath->setPingPong(true);
    planetPath->setSpeed(0.002f);

    // Create planet (using existing sphere model)
    orbitingPlanet = new DrawableObject(sphereModel, phongShader);
    orbitingPlanet->setColor(glm::vec3(0.8f, 0.3f, 0.1f));  // Orange/red planet
    orbitingPlanet->scale(glm::vec3(2.5f));  // Larger than fireflies
    // Position will be updated in renderFrame()
    om->addDrawableObject(orbitingPlanet);

    printf("Bezier planet created!\n");

    printf("Total: %d objects\n", om->getObjectCount());
}

void MinimalForestScene::callbacks()
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

void MinimalForestScene::createScene(GLFWwindow* window)
{
    printf("\n========================================\n");
    printf("   CREATING FOREST SCENE\n");
    printf("========================================\n\n");

    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);

    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();
    this->tm = new TextureManager();
    this->mm = new MaterialManager();

    this->interactionManager = new ObjectInteractionManager(om, camera, window);

    createShaders();
    createTextures();
    createMaterials();
    createLights();
    createDrawableObjects();
    callbacks();

    printf("\n========================================\n");
    printf("   SCENE READY\n");
    printf("========================================\n\n");
}

void MinimalForestScene::handleFlashlightInput()
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!keyFWasPressed)
        {
            flashlightEnabled = !flashlightEnabled;

            if (flashlightEnabled)
            {
                lm->addSpotlightCamera(flashlight);
                printf("Flashlight ON\n");
            }
            else
            {
                lm->removeSpotlightCamera();
                printf("Flashlight OFF\n");
            }

            spm->updateLights();
            keyFWasPressed = true;
        }
    }
    else
    {
        keyFWasPressed = false;
    }
}

void MinimalForestScene::renderFrame()
{
    camera->checkChanges();
    camera->controls();
    handleFlashlightInput();
    handleMouseInput();

    float time = static_cast<float>(glfwGetTime());
    for (Firefly* firefly : fireflies)
    {
        firefly->updateAnimation(time);
    }

    // Update orbiting planet position on Bezier spline
    if (orbitingPlanet && planetPath)
    {
        // Update spline animation
        planetPath->update(0.016f);

        // Get current position on spline
        glm::vec3 planetPosition = planetPath->calculatePoint();

        // Apply transformations (calculateModelMatrix() will auto-clear previous ones)
        orbitingPlanet->scale(glm::vec3(2.5f));
        orbitingPlanet->translate(planetPosition);
        orbitingPlanet->calculateModelMatrix();
        orbitingPlanet->updateModelMatrix();
    }

    spm->updateLights();
    om->drawSkybox();
    om->drawObjects();
}

void MinimalForestScene::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    // Stencil buffer for picking
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    printf("========================================\n");
    printf("CONTROLS:\n");
    printf("  W/A/S/D     - Move\n");
    printf("  Space/Shift - Up/Down\n");
    printf("  Right Mouse - Look\n");
    printf("  F           - Flashlight\n");
    printf("  LEFT CLICK  - Place tree\n");
    printf("  MIDDLE CLICK- Remove object\n");
    printf("  ESC         - Exit\n");
    printf("========================================\n");

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

// ========================================
// MOUSE INTERACTION - PICKING & PLACING
// ========================================

void MinimalForestScene::handleMouseInput()
{
    // Left click - place tree
    if (Callback::hasLeftClick())
    {
        glm::vec3 screenPos = Callback::position;
        glm::vec3 worldPos = interactionManager->screenToWorld(screenPos);
        placeTreeAt(worldPos);
        Callback::clearClicks();
    }

    // Middle click - remove object
    if (Callback::hasMiddleClick())
    {
        int stencilID = Callback::getClickedObjectID();
        removeObjectAt(stencilID);
        Callback::clearClicks();
    }
}

void MinimalForestScene::placeTreeAt(glm::vec3 worldPos)
{
    printf("[LEFT CLICK] Placing tree at (%.2f, %.2f, %.2f)\n", worldPos.x, worldPos.y, worldPos.z);

    TreeModel* newTreeModel = new TreeModel();
    ShaderProgram* phongShader = spm->getShaderProgram(1);

    DrawableObject* newTree = new DrawableObject(newTreeModel, phongShader);
    newTree->setColor(glm::vec3(0.1f, 0.3f, 0.05f));
    interactionManager->placeObject(newTree, worldPos);

    printf("Tree placed! Total objects: %d\n", om->getObjectCount());
}

void MinimalForestScene::removeObjectAt(int stencilID)
{
    printf("[MIDDLE CLICK] Stencil ID: %d\n", stencilID);
    interactionManager->removeObject(stencilID);
}
