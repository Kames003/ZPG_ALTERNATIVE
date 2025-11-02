#include "MinimalForestScene.h"
#include "TreeModel.h"
#include "PlainModel.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotlightCamera.h"
#include "BenchModel.h"
#include "BushModel.h"
#include "SphereModel.h"
#include "Firefly.h"
#include <cstdlib>
#include "LoadedModel.h"
#include <ctime>

void MinimalForestScene::createShaders()
{
    printf("  Creating shaders...\n");

    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    spm->addShaderProgram(camera,
        "Shaders/VertexShaderConstant.glsl",
        "Shaders/FragmentShaderConstant.glsl");

    printf("    ✅ Phong shader (index 0) - 16 lights + flashlight\n");
    printf("    ✅ Constant shader (index 1) - fireflies\n");
}

void MinimalForestScene::createLights()
{
    printf("Starting to create lights...\n");

    DirectionalLight* moonLight = new DirectionalLight(
        glm::normalize(glm::vec3(0.3f, -1.0f, 0.2f)),
        glm::vec3(0.08f, 0.08f, 0.12f)
    );

    lm->addDirectionalLight(moonLight);
    lm->ambient = 0.10f;

    flashlight = new SpotlightCamera(
        1.0f,
        0.09f,
        0.032f,
        cos(glm::radians(12.5f)),
        glm::vec3(1.0f, 1.0f, 0.9f)
    );

    printf("Flashlight created (press F to toggle)\n");

    spm->updateLights();

    printf("DirectionalLight moonlight created\n");
    printf("Ambient of scene is : 0.20\n");
}

void MinimalForestScene::createDrawableObjects()
{
    printf("  Creating objects...\n");
    srand(static_cast<unsigned>(time(nullptr)));

    ShaderProgram* phongShader = spm->getShaderProgram(0);
    ShaderProgram* constantShader = spm->getShaderProgram(1);

    // ========== ZEMA ==========
    PlainModel* groundModel = new PlainModel();
    DrawableObject* ground = new DrawableObject(groundModel, phongShader);
    ground->setColor(glm::vec3(0.15f, 0.25f, 0.15f));
    ground->scale(glm::vec3(100.0f, 1.0f, 100.0f));
    ground->translate(glm::vec3(0.0f, -0.5f, 0.0f));
    ground->calculateModelMatrix();
    ground->updateModelMatrix();
    om->addDrawableObject(ground);

    // ========== CESTA ==========
    DrawableObject* path = new DrawableObject(groundModel, phongShader);
    path->setColor(glm::vec3(0.6f, 0.5f, 0.4f));
    path->scale(glm::vec3(3.0f, 1.0f, 100.0f));
    path->translate(glm::vec3(0.0f, -0.4f, 0.0f));
    path->calculateModelMatrix();
    path->updateModelMatrix();
    om->addDrawableObject(path);

    // ========== STROMY ==========
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
    printf("      ✅ 70 trees\n");

    // ========== LAVIČKY ==========
    BenchModel* benchModel = new BenchModel();
    float benchPositions[][3] = {
        {-5.0f, -0.5f, -10.0f},
        {5.0f, -0.5f, -10.0f},
        {-5.0f, -0.5f, 10.0f},
        {5.0f, -0.5f, 10.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        DrawableObject* bench = new DrawableObject(benchModel, phongShader);
        bench->setColor(glm::vec3(0.55f, 0.17f, 0.05f));
        bench->translate(glm::vec3(benchPositions[i][0],
                                   benchPositions[i][1],
                                   benchPositions[i][2]));
        bench->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        bench->scale(glm::vec3(1.5f));
        bench->calculateModelMatrix();
        bench->updateModelMatrix();
        om->addDrawableObject(bench);
    }
    printf("      ✅ 4 benches\n");

    // ========================================
    // ✅ ZMENA 1: LAMPY S MENŠOU SVIETIVOSŤOU
    // ========================================
    printf("  Creating lamps (with reduced light intensity)...\n");
    LoadedModel* lampModel = new LoadedModel("Models/lamp.obj");

    float lampPositions[][3] = {
        {-2.5f, 0.32f, -20.0f},
        {2.5f, 0.32f, -20.0f},
        {-2.5f, 0.32f, -10.0f},
        {2.5f, 0.32f, -10.0f},
        {-2.5f, 0.32f, 0.0f},
        {2.5f, 0.32f, 0.0f},
        {-2.5f, 0.32f, 10.0f},
        {2.5f, 0.32f, 10.0f},
        {-2.5f, 0.32f, 20.0f},
        {2.5f, 0.32f, 20.0f}
    };

    int numLamps = 10;

    for (int i = 0; i < numLamps; i++)
    {
        // Vytvor lampu (vizualizáciu)
        DrawableObject* lamp = new DrawableObject(lampModel, phongShader);
        lamp->setColor(glm::vec3(0.9f, 0.8f, 0.3f));
        lamp->translate(glm::vec3(lampPositions[i][0],
                                  lampPositions[i][1],
                                  lampPositions[i][2]));
        lamp->scale(glm::vec3(0.23f));
        lamp->calculateModelMatrix();
        lamp->updateModelMatrix();
        om->addDrawableObject(lamp);

        // ✅ ZMENA: Slabšie svetlo - len pre efekt!
        PointLight* lampLight = new PointLight(
            glm::vec3(lampPositions[i][0],
                     lampPositions[i][1] + 1.5f,
                     lampPositions[i][2]),
            1.0f,     // constant
            0.7f,     // ✅ ZVÝŠENÝ linear útlum (bolo 0.3)
            1.2f,     // ✅ ZVÝŠENÝ quadratic útlum (bolo 0.3) - dosah ~2-3m
            glm::vec3(1.0f, 0.9f, 0.7f) * 0.4f  // ✅ Farebná intenzita znížená na 40%
        );
        lm->addPointLight(lampLight);
    }

    printf("      ✅ %d lamps with REDUCED light intensity (decorative)\n", numLamps);

    // ========================================
    // ✅ ZMENA 2: PRIDANIE SHUTTLE MODELU (HAVARUJÚCA LOĎKA)
    // ========================================
    printf("  Creating shuttle (crashing spaceship)...\n");
    LoadedModel* shuttleModel = new LoadedModel("Models/shuttle.obj");

    DrawableObject* shuttle = new DrawableObject(shuttleModel, phongShader);
    shuttle->setColor(glm::vec3(0.7f, 0.7f, 0.75f));  // Kovová sivá farba

    // ✅ Umiestnenie nad scénou - havarujúca loďka!
    shuttle->translate(glm::vec3(-15.0f, 25.0f, 10.0f));  // Vľavo, vysoko, vpredu

    // ✅ Sklony pre efekt havarie
    shuttle->rotate(-25.0f, glm::vec3(1.0f, 0.0f, 0.0f));  // Sklon dopredu (nos dole)
    shuttle->rotate(15.0f, glm::vec3(0.0f, 0.0f, 1.0f));   // Mierne naklopenie do strany
    shuttle->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));   // Rotácia okolo Y (smer letu)

    shuttle->scale(glm::vec3(0.5f));  // Menšia veľkosť

    shuttle->calculateModelMatrix();
    shuttle->updateModelMatrix();
    om->addDrawableObject(shuttle);

    printf("      ✅ Shuttle loaded - crashing above forest!\n");

    // ========== KRÍKY ==========
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
    printf("      ✅ 15 bushes\n");

    // ========== SVETLUŠKY ==========
    printf("  Creating fireflies...\n");
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
        if (colorType == 0) {
            color = glm::vec3(1.0f, 0.9f, 0.5f);
        } else if (colorType == 1) {
            color = glm::vec3(0.7f, 1.0f, 0.6f);
        } else {
            color = glm::vec3(1.0f, 0.95f, 0.7f);
        }

        Firefly* firefly = new Firefly(
            glm::vec3(x, y, z),
            sphereModel,
            constantShader,
            color
        );

        lm->addPointLight(firefly->getLight());
        om->addDrawableObject(firefly->getVisual());
        fireflies.push_back(firefly);
    }

    printf("      ✅ 12 fireflies\n");
    printf("    ✅ Total: %d objects\n", om->getObjectCount());
}

void MinimalForestScene::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void MinimalForestScene::createScene(GLFWwindow* window)
{
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║   CREATING FOREST SCENE               ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);

    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    createShaders();
    createLights();
    createDrawableObjects();
    callbacks();

    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║         SCENE READY                   ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
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
                printf("\n🔦 Flashlight ON\n");
            }
            else
            {
                lm->removeSpotlightCamera();
                printf("\n🔦 Flashlight OFF\n");
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

    float time = static_cast<float>(glfwGetTime());
    for (Firefly* firefly : fireflies)
    {
        firefly->updateAnimation(time);
    }

    spm->updateLights();
    om->drawObjects();
}

void MinimalForestScene::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    printf("╔═════════════════════════════════════════╗\n");
    printf("║       RENDER LOOP                     ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║ W/A/S/D     - Move                    ║\n");
    printf("║ Space/Shift - Up/Down                 ║\n");
    printf("║ Right Mouse - Look                    ║\n");
    printf("║ F           - Flashlight ON/OFF       ║\n");
    printf("║ ESC         - Exit                    ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║ ✅ Lamps: Reduced intensity (deco)    ║\n");
    printf("║ 🚀 Shuttle: Crashing above forest!    ║\n");
    printf("╚═════════════════════════════════════════╝\n\n");

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