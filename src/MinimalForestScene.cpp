#include "MinimalForestScene.h"
#include "TreeModel.h"
#include "PlainModel.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include "SpotlightCamera.h"
#include "BenchModel.h"
#include "BushModel.h"
#include "SphereModel.h"
#include "Firefly.h"
#include <cstdlib>
#include <ctime>

void MinimalForestScene::createShaders()
{
    printf("  Creating shaders...\n");

    // ✅ PHONG SHADER - s 16 svetlami + baterka
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    // Constant shader pre svetlušky
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderConstant.glsl",
        "Shaders/FragmentShaderConstant.glsl");

    printf("    ✅ Phong shader (index 0) - 16 lights + flashlight\n");
    printf("    ✅ Constant shader (index 1) - fireflies\n");
}

void MinimalForestScene::createLights()
{
    printf("  Creating lights...\n");

    // ========================================
    // MESAČNÉ SVETLO
    // ========================================
    PointLight* moonLight = new PointLight(
        glm::vec3(0.0f, 50.0f, 0.0f),
        1.0f, 0.0f, 0.005f,
        glm::vec3(0.3f, 0.3f, 0.4f)
    );

    lm->addPointLight(moonLight);
    lm->ambient = 0.20f;

    // ========================================
    // 🔦 BATERKA (SpotlightCamera)
    // ========================================
    flashlight = new SpotlightCamera(
        1.0f,      // constant
        0.09f,     // linear
        0.032f,    // quadratic
        cos(glm::radians(12.5f)),  // angle (25°)
        glm::vec3(1.0f, 1.0f, 0.9f)
    );

    printf("    🔦 Flashlight created (press F to toggle)\n");

    // ✅ NEINICIALIZUJEME baterku hneď (zapne sa klávesou F)
    spm->updateLights();

    printf("    ✅ Moon light at (0, 50, 0)\n");
    printf("    ✅ Ambient: 0.20\n");
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
        bench->setColor(glm::vec3(0.35f, 0.17f, 0.05f));
        bench->translate(glm::vec3(benchPositions[i][0],
                                   benchPositions[i][1],
                                   benchPositions[i][2]));
        bench->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        bench->scale(glm::vec3(0.5f));
        bench->calculateModelMatrix();
        bench->updateModelMatrix();
        om->addDrawableObject(bench);
    }
    printf("      ✅ 4 benches\n");

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

    // ========================================
    // SVETLUŠKY
    // ========================================
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

    // ✅ NASTAV KAMERU VYŠŠIE (2 metre nad zemou)
    //camera->setCameraPosition(glm::vec3(0.0f, 2.0f, 10.0f));

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

// ========================================
// 🔦 OBSLUHA KLÁVESY F (BATERKA ON/OFF)
// ========================================
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
                // ✅ OPRAVENÉ: Použij novú metódu
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

    // 🔦 BATERKA
    handleFlashlightInput();

    // ANIMÁCIA SVETLUŠIEK
    float time = static_cast<float>(glfwGetTime());
    for (Firefly* firefly : fireflies)
    {
        firefly->updateAnimation(time);
    }

    // ✅ DEBUG: Vypíš info o svetlách (len raz za sekundu)
    static float lastPrintTime = 0.0f;
    if (time - lastPrintTime > 1.0f)
    {
        printf("\n=== DEBUG INFO ===\n");
        printf("Number of point lights: %d\n", lm->getPointLightsSize());
        printf("Camera position: (%.2f, %.2f, %.2f)\n",
               camera->getCameraPosition().x,
               camera->getCameraPosition().y,
               camera->getCameraPosition().z);
        printf("Ambient: %.2f\n", lm->ambient);

        if (lm->getPointLightsSize() > 0) {
            PointLight* first = lm->getPointLight(0);
            printf("First light position: (%.2f, %.2f, %.2f)\n",
                   first->position.x, first->position.y, first->position.z);
            printf("First light color: (%.2f, %.2f, %.2f)\n",
                   first->color.r, first->color.g, first->color.b);
        }

        lastPrintTime = time;
    }

    // AKTUALIZUJ SVETLÁ
    spm->updateLights();

    // Vykresli
    om->drawObjects();
}

void MinimalForestScene::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    printf("╔═══════════════════════════════════════╗\n");
    printf("║       RENDER LOOP                     ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║ W/A/S/D     - Move                    ║\n");
    printf("║ Space/Shift - Up/Down                 ║\n");
    printf("║ Right Mouse - Look                    ║\n");
    printf("║ F           - Flashlight ON/OFF       ║\n");
    printf("║ ESC         - Exit                    ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

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