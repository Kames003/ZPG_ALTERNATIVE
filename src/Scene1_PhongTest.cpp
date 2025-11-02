#include "Scene1_PhongTest.h"

void Scene1_PhongTest::createShaders()
{
    // Phong shader
    spm->addShaderProgram(camera, 
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");
}

void Scene1_PhongTest::createDrawableObjects()
{
    TriangleModel* triangle = new TriangleModel();

    // Shader
    ShaderProgram* phongShader = spm->getShaderProgram(0);

    DrawableObject* obj = new DrawableObject(
        triangle,  // ✅ Triangle!
        phongShader,
        glm::vec3(0.8f, 0.2f, 0.2f)  // Červená farba
    );


    obj->translate(glm::vec3(0.0f, 0.0f, 0.0f));
    obj->scale(glm::vec3(1.0f, 1.0f, 1.0f));  // ✅ Väčší scale pre trojuholník
    obj->calculateModelMatrix();
    obj->updateModelMatrix();


    om->addDrawableObject(obj);

    printf("  Scene 1: Created 1 rotating TRIANGLE (Phong, red)\n");
}

void Scene1_PhongTest::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene1_PhongTest::createScene(GLFWwindow* window)
{
    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // ========================================
    // 3 SVETLÁ s rôznymi farbami
    // ========================================

    // Svetlo 1: Biele v strede
    PointLight* light1 = new PointLight(
        glm::vec3(0.0f, 0.0f, 2.0f),
        1.0f, 0.0f, 0.5f,
        glm::vec3(1.0f, 1.0f, 1.0f)  // Biele
    );
    lm->addPointLight(light1);

    // Svetlo 2: Červenkasté vpravo hore
    PointLight* light2 = new PointLight(
        glm::vec3(5.0f, 3.0f, 2.0f),
        1.0f, 0.0f, 0.5f,
        glm::vec3(1.0f, 0.5f, 0.5f)  // Červenkasté
    );
    lm->addPointLight(light2);

    // Svetlo 3: Modrasté vľavo hore
    PointLight* light3 = new PointLight(
        glm::vec3(-5.0f, 3.0f, 2.0f),
        1.0f, 0.0f, 0.5f,
        glm::vec3(0.5f, 0.5f, 1.0f)  // Modrasté
    );
    lm->addPointLight(light3);

    lm->ambient = 0.2f;
    spm->updateLights();

    printf("  Scene 1: Created 3 lights (white, reddish, bluish)\n");
}

void Scene1_PhongTest::renderFrame()
{
    camera->checkChanges();
    camera->controls();

    // ========================================
    // ANIMÁCIA - rotácia každý frame
    // ========================================
    DrawableObject* obj = om->getDrawableObject(0);

    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) rotationAngle = 0.0f;

    // KĽÚČOVÉ: Pridaj transformácie ZNOVA každý frame
    obj->translate(glm::vec3(0.0f, 0.0f, 0.0f));
    obj->scale(glm::vec3(1.0f, 1.0f, 1.0f));
    obj->rotate(rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));  // ✅ Rotácia okolo Z (v rovine obrazovky)
    
    // Vypočítaj (VYPRÁZDNI Composite!) a pošli do shadera
    obj->calculateModelMatrix();
    obj->updateModelMatrix();

    // Vykresli
    om->drawObjects();
}

void Scene1_PhongTest::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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