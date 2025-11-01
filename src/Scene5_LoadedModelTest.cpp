// Scene5_LoadedModelTest.cpp - TEAPOT + PYRAMID
#include "Scene5_LoadedModelTest.h"
#include "DrawableObject.h"

void Scene5_LoadedModelTest::createShaders()
{
    // ✅ POUŽIJ PHONG SHADER
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");
}

void Scene5_LoadedModelTest::createDrawableObjects()
{
    printf("  Loading external models...\n");

    ShaderProgram* phongShader = spm->getShaderProgram(0);

    // ========================================
    // MODEL 1: TEAPOT (vľavo)
    // ========================================
    printf("  🫖 Loading teapot...\n");
    LoadedModel* teapotModel = new LoadedModel("Models/teapot.obj");

    DrawableObject* teapot = new DrawableObject(
       teapotModel,
       phongShader,
       glm::vec3(0.8f, 0.6f, 0.4f)  // Hnedá farba
    );

    teapot->translate(glm::vec3(-3.5f, -1.0f, 0.0f));  // ✅ Vľavo dole
    teapot->scale(glm::vec3(0.1f));
    teapot->calculateModelMatrix();
    teapot->updateModelMatrix();
    om->addDrawableObject(teapot);

    printf("    ✅ Teapot loaded (left)\n");

    // ========================================
    // MODEL 2: LAMP (stred)
    // ========================================
    printf("  💡 Loading lamp...\n");
    LoadedModel* lampModel = new LoadedModel("Models/lamp.obj");

    DrawableObject* lamp = new DrawableObject(
       lampModel,
       phongShader,
       glm::vec3(0.9f, 0.8f, 0.3f)  // Zlatá farba
    );

    lamp->translate(glm::vec3(10.0f, 0.0f, 0.0f));  // ✅ V strede
    lamp->scale(glm::vec3(0.2f));  // Trochu menšia
    lamp->calculateModelMatrix();
    lamp->updateModelMatrix();
    om->addDrawableObject(lamp);

    printf("    ✅ Lamp loaded (center)\n");


}

void Scene5_LoadedModelTest::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene5_LoadedModelTest::createScene(GLFWwindow* window)
{
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║   CREATING TWO MODEL TEST SCENE       ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // ========================================
    // SVETLÁ - 2 bodové svetlá pre lepšie osvetlenie
    // ========================================

    // Hlavné svetlo (vpredu, hore)
    PointLight* light1 = new PointLight(
        glm::vec3(0.0f, 5.0f, 5.0f),
        1.0f, 0.0f, 0.2f,
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    lm->addPointLight(light1);

    // Vedľajšie svetlo (z boku)
    PointLight* light2 = new PointLight(
        glm::vec3(5.0f, 2.0f, 0.0f),
        1.0f, 0.0f, 0.3f,
        glm::vec3(0.8f, 0.9f, 1.0f)  // Mierne modrasté
    );
    lm->addPointLight(light2);

    lm->ambient = 0.3f;
    spm->updateLights();

    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║         SCENE READY                   ║\n");
    printf("║                                       ║\n");
    printf("║  🫖 Teapot (left)  🔺 Pyramid (right) ║\n");
    printf("║                                       ║\n");
    printf("║  W/S/A/D - Move camera                ║\n");
    printf("║  Right Mouse - Look around            ║\n");
    printf("║  ESC - Exit                           ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
}

void Scene5_LoadedModelTest::renderFrame()
{
    camera->checkChanges();
    camera->controls();
    om->drawObjects();
}

void Scene5_LoadedModelTest::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // Mierne tmavšie pozadie

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