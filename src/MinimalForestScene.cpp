// MinimalForestScene.cpp
#include "MinimalForestScene.h"
#include "TreeModel.h"
#include "PlainModel.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include <cstdlib>
#include "BenchModel.h"      // ← PRIDAJ
#include "BushModel.h"
#include <ctime>

void MinimalForestScene::createShaders()
{
    printf("  Creating shaders...\n");

    // Jediný shader program s Phong osvetlením
    spm->addShaderProgram(camera,
        "Shaders/simple_forest_vertex.glsl",      // Vertex shader
        "Shaders/simple_forest_fragment.glsl");    // Fragment shader

    printf("    ✅ Phong shader created (index 0)\n");
}

void MinimalForestScene::createLights()
{
    printf("  Creating lights...\n");

    // ========================================
    // JEDNO BODOVÉ SVETLO NAD SCÉNOU
    // ========================================
    PointLight* mainLight = new PointLight(
        glm::vec3(0.0f, 15.0f, 0.0f),  // Pozícia vysoko nad scénou
        1.0f,    // constant
        0.0f,    // linear
        0.01f,   // quadratic
        glm::vec3(1.0f, 1.0f, 0.9f)    // Teplá biela farba
    );

    lm->addPointLight(mainLight);
    lm->ambient = 0.3f;  // Zvýšený ambient pre lepšiu viditeľnosť

    // ========================================
    // AKTUALIZUJ SVETLÁ V SHADEROCH
    // ========================================
    // KRITICKÉ: Musíme poslać údaje o svetlách do uniformov
    ShaderProgram* shader = spm->getShaderProgram(0);
    shader->activateShaderProgram();

    // Počet svetiel
    shader->updateUniform("numberOfLights", 1);

    // Údaje prvého svetla
    shader->updateUniform("lights[0].position",
        glm::vec4(mainLight->position, 1.0f));
    shader->updateUniform("lights[0].ambient",
        glm::vec3(0.2f, 0.2f, 0.2f));
    shader->updateUniform("lights[0].diffuse",
        mainLight->color);
    shader->updateUniform("lights[0].specular",
        glm::vec3(1.0f, 1.0f, 1.0f));
    shader->updateUniform("lights[0].intensity", 1.0f);

    shader->deactivateShaderProgram();

    printf("    ✅ 1 point light created at (0, 15, 0)\n");
    printf("    ✅ Ambient: 0.3\n");
}

void MinimalForestScene::createDrawableObjects()
{
    printf("  Creating objects...\n");
    srand(static_cast<unsigned>(time(nullptr)));
    ShaderProgram* phongShader = spm->getShaderProgram(0);

    // ========== ROVINA (ZEM) ==========
    PlainModel* groundModel = new PlainModel();
    DrawableObject* ground = new DrawableObject(groundModel, phongShader);
    ground->setColor(glm::vec3(0.2f, 0.5f, 0.2f));
    ground->scale(glm::vec3(100.0f, 1.0f, 100.0f));
    ground->translate(glm::vec3(0.0f, -0.5f, 0.0f));
    ground->calculateModelMatrix();
    ground->updateModelMatrix();
    om->addDrawableObject(ground);

    // ========== PIESKOVÁ CESTA ==========
    DrawableObject* path = new DrawableObject(groundModel, phongShader);
    path->setColor(glm::vec3(0.87f, 0.72f, 0.53f));
    path->scale(glm::vec3(3.0f, 1.0f, 100.0f));
    path->translate(glm::vec3(0.0f, -0.4f, 0.0f));
    path->calculateModelMatrix();
    path->updateModelMatrix();
    om->addDrawableObject(path);

    // ========== 70 STROMOV - NIŽŠIE ==========
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
        tree->setColor(glm::vec3(0.2f, 0.5f, 0.1f));

        // ✅ Y = -0.5f (rovnaká úroveň ako zem)
        tree->translate(glm::vec3(x, -0.5f, z));
        tree->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        tree->scale(glm::vec3(scale));
        tree->calculateModelMatrix();
        tree->updateModelMatrix();
        om->addDrawableObject(tree);
    }
    printf("      ✅ 70 trees created\n");

    // ========== LAVIČKY - NIŽŠIE ==========
    BenchModel* benchModel = new BenchModel();
    float benchPositions[][3] = {
        {-5.0f, -0.5f, -10.0f},   // ✅ Y = -0.5f
        {5.0f, -0.5f, -10.0f},
        {-5.0f, -0.5f, 10.0f},
        {5.0f, -0.5f, 10.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        DrawableObject* bench = new DrawableObject(benchModel, phongShader);
        bench->setColor(glm::vec3(0.55f, 0.27f, 0.07f));
        bench->translate(glm::vec3(benchPositions[i][0],
                                   benchPositions[i][1],
                                   benchPositions[i][2]));
        bench->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        bench->scale(glm::vec3(0.5f));
        bench->calculateModelMatrix();
        bench->updateModelMatrix();
        om->addDrawableObject(bench);
    }
    printf("      ✅ 4 benches created\n");

    // ========== KRÍKY - NIŽŠIE ==========
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
        bush->setColor(glm::vec3(0.3f, 0.6f, 0.2f));

        // ✅ Y = -0.5f (rovnaká úroveň ako zem)
        bush->translate(glm::vec3(x, -0.5f, z));
        bush->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        bush->scale(glm::vec3(scale));
        bush->calculateModelMatrix();
        bush->updateModelMatrix();
        om->addDrawableObject(bush);
    }
    printf("      ✅ 15 bushes created\n");
    printf("    ✅ Total objects: %d\n", om->getObjectCount());
}

void MinimalForestScene::callbacks()
{
    // Callback pre pohyb myši
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    // Callback pre zmenu veľkosti okna
    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void MinimalForestScene::createScene(GLFWwindow* window)
{
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   CREATING MINIMAL FOREST SCENE       ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    this->window = window;

    // Vytvor kameru
    this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);

    // Vytvor managery
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    // Inicializuj scénu
    createShaders();
    createLights();
    createDrawableObjects();
    callbacks();

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║         SCENE READY TO RENDER          ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
}

void MinimalForestScene::renderScene()
{
    // OpenGL nastavenia
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // Svetlo modrá obloha

    printf("╔════════════════════════════════════════╗\n");
    printf("║       STARTING RENDER LOOP             ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ Controls:                              ║\n");
    printf("║  W/A/S/D     - Move camera             ║\n");
    printf("║  Space/Shift - Up/Down                 ║\n");
    printf("║  Right Mouse - Look around             ║\n");
    printf("║  ESC         - Exit                    ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    // Hlavný render loop
    while (!glfwWindowShouldClose(window))
    {
        // Vymaž buffery
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Aktualizuj kameru
        camera->checkChanges();
        camera->controls();

        // Vykresli všetky objekty
        om->drawObjects();

        // Swap buffers a spracuj udalosti
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Upratanie
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}