// Scene7_NormalMapTest.cpp - Test normálových map
#include "Scene7_NormalMapTest.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include "Texture2D.h"

void Scene7_NormalMapTest::createShaders()
{
    // Shader s normálovou mapou
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongNormalMap.glsl",
        "Shaders/FragmentShaderPhongNormalMap.glsl");
}

void Scene7_NormalMapTest::createDrawableObjects()
{
    printf("  Loading models with normal maps...\n");

    ShaderProgram* normalMapShader = spm->getShaderProgram(0);


    tm->addTexture(new Texture2D("models/NormalMapTest/albedo.png"));      // 0 - Diffuse
    tm->addTexture(new Texture2D("models/NormalMapTest/normalmap.png"));   // 1 - Normal Map

    tm->addTexture(new Texture2D("models/NormalMapTest/Sphere/albedo.png"));      // 2
    tm->addTexture(new Texture2D("models/NormalMapTest/Sphere/normalmap.png"));   // 3

    // ========================================
    // TEXTÚRY - Box
    // ========================================
    tm->addTexture(new Texture2D("models/NormalMapTest/Box/albedo.png"));         // 4
    tm->addTexture(new Texture2D("models/NormalMapTest/Box/normalmap.png"));      // 5

    // ========================================
    // MODEL 1: SPHERE (vľavo)
    // ========================================
    printf("Loading Sphere with normal map...\n");
    LoadedModel* sphereModel = new LoadedModel("models/NormalMapTest/Sphere/model.obj");

    DrawableObject* sphere = new DrawableObject(
       sphereModel,
       normalMapShader,
       glm::vec3(1.0f, 1.0f, 1.0f)
    );

    sphere->addTexture(tm->getTexture(2));  // Diffuse texture
    sphere->translate(glm::vec3(-3.0f, 0.0f, 0.0f));
    sphere->scale(glm::vec3(1.5f));
    sphere->calculateModelMatrix();
    sphere->updateModelMatrix();
    om->addDrawableObject(sphere);

    printf("Sphere loaded (left)\n");

    // ========================================
    // MODEL 2: MAIN MODEL (stred)
    // ========================================
    printf("Loading main model with normal map...\n");
    LoadedModel* mainModel = new LoadedModel("models/NormalMapTest/model.obj");

    DrawableObject* mainObj = new DrawableObject(
       mainModel,
       normalMapShader,
       glm::vec3(1.0f, 1.0f, 1.0f)
    );

    mainObj->addTexture(tm->getTexture(0));  // Diffuse texture
    mainObj->translate(glm::vec3(0.0f, 0.0f, 0.0f));
    mainObj->scale(glm::vec3(1.0f));
    mainObj->calculateModelMatrix();
    mainObj->updateModelMatrix();
    om->addDrawableObject(mainObj);

    printf("Main model loaded (center)\n");

    // ========================================
    // MODEL 3: BOX (vpravo)
    // ========================================
    printf("Loading Box with normal map...\n");
    LoadedModel* boxModel = new LoadedModel("models/NormalMapTest/Box/model.obj");

    DrawableObject* box = new DrawableObject(
       boxModel,
       normalMapShader,
       glm::vec3(1.0f, 1.0f, 1.0f)
    );

    box->addTexture(tm->getTexture(4));  // Diffuse texture
    box->translate(glm::vec3(3.0f, 0.0f, 0.0f));
    box->scale(glm::vec3(1.0f));
    box->calculateModelMatrix();
    box->updateModelMatrix();
    om->addDrawableObject(box);

    printf("Box loaded (right)\n");
}

void Scene7_NormalMapTest::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene7_NormalMapTest::createScene(GLFWwindow* window)
{
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║   NORMAL MAP TEST SCENE               ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();
    this->tm = new TextureManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // ========================================
    // SVETLÁ - Silné osvětlení pro zvýraznění normálových map
    // ========================================

    // Hlavné svetlo (vpredu nahor) - silné biele svetlo
    PointLight* light1 = new PointLight(
        glm::vec3(0.0f, 8.0f, 15.0f),  // Vyššie a ďalej
        1.0f, 0.01f, 0.02f,            // Silnejšie (menšia atenuácia)
        glm::vec3(1.5f, 1.5f, 1.5f)    // Jasnejšie biele svetlo
    );
    lm->addPointLight(light1);

    // Vedľajšie svetlo (z boku) - pre lepšie zvýraznenie nerovností
    PointLight* light2 = new PointLight(
        glm::vec3(15.0f, 5.0f, 5.0f),  // Z boku
        1.0f, 0.02f, 0.03f,
        glm::vec3(1.2f, 1.2f, 1.2f)    // Silné svetlo
    );
    lm->addPointLight(light2);

    // Tretie svetlo (zpredu) - pre vyplnenie tieňov
    PointLight* light3 = new PointLight(
        glm::vec3(-10.0f, 3.0f, 8.0f),
        1.0f, 0.03f, 0.04f,
        glm::vec3(0.8f, 0.8f, 0.9f)    // Mierne chladnejšie
    );
    lm->addPointLight(light3);

    lm->ambient = 0.05f;  // VEĽMI nízky ambient pre silný kontrast
    spm->updateLights();

    printf("  💡 Lighting setup: 3 strong lights, ambient=0.05, normalMapIntensity=3.0\n");

    // ========================================
    // NASTAVENIE NORMÁLOVÝCH MAP V SHADEROCH
    // ========================================
    ShaderProgram* normalMapShader = spm->getShaderProgram(0);
    normalMapShader->activateShaderProgram();


    normalMapShader->updateUniform("tex", 0);           // Diffuse texture na GL_TEXTURE0
    normalMapShader->updateUniform("normalMap", 1);     // Normal map na GL_TEXTURE1
    normalMapShader->updateUniform("normalMapIntensity", 3.0f);

    normalMapShader->deactivateShaderProgram();

    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║         SCENE READY                   ║\n");
    printf("║                                       ║\n");
    printf("║  Sphere (left) Model (center) Box (right) ║\n");
    printf("║                                       ║\n");
    printf("║  W/S/A/D - Move camera                ║\n");
    printf("║  Right Mouse - Look around            ║\n");
    printf("║  ESC - Exit                           ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
}

void Scene7_NormalMapTest::renderFrame()
{
    camera->checkChanges();
    camera->controls();

    for (int i = 0; i < 3; i++) {
        DrawableObject* obj = om->getDrawableObject(i);

        if (obj->textures.size() > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj->textures[0]->getID());
        }

        GLuint normalMapID = 0;
        if (i == 0) normalMapID = tm->getTexture(3)->getID();  // Sphere normal map
        else if (i == 1) normalMapID = tm->getTexture(1)->getID();  // Main model normal map
        else if (i == 2) normalMapID = tm->getTexture(5)->getID();  // Box normal map

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapID);

        obj->drawModel();
    }
}


void Scene7_NormalMapTest::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Čisto čierne pozadie pre lepší kontrast

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