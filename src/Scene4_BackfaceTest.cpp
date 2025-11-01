#include "Scene4_BackfaceTest.h"

void Scene4_BackfaceTest::createShaders()
{
    // Shader 0: Phong SPRÁVNY (max, kontrola if)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhongCorrect.glsl");  // ✅ Správny

    // Shader 1: Phong ZLÝ (abs, bez kontroly)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhongWrong.glsl");    // ❌ Zlý
}

void Scene4_BackfaceTest::createDrawableObjects()
{
    SphereModel* sphereModel = new SphereModel();

    ShaderProgram* correctShader = spm->getShaderProgram(0);
    ShaderProgram* wrongShader = spm->getShaderProgram(1);

    // ========================================
    // KULIČKA 1 (HORE): SPRÁVNY SHADER - ČERVENÁ
    // ========================================
    DrawableObject* correctSphere = new DrawableObject(
        sphereModel,
        correctShader,
        glm::vec3(0.8f, 0.2f, 0.2f)  // Červená
    );
    correctSphere->translate(glm::vec3(0.0f, 1.0f, 0.0f));
    correctSphere->scale(glm::vec3(0.5f));
    correctSphere->calculateModelMatrix();
    correctSphere->updateModelMatrix();
    om->addDrawableObject(correctSphere);

    // ========================================
    // KULIČKA 2 (DOLE): ZLÝ SHADER - ZELENÁ
    // ========================================
    DrawableObject* wrongSphere = new DrawableObject(
        sphereModel,
        wrongShader,
        glm::vec3(0.2f, 0.8f, 0.2f)  // Zelená
    );
    wrongSphere->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    wrongSphere->scale(glm::vec3(0.5f));
    wrongSphere->calculateModelMatrix();
    wrongSphere->updateModelMatrix();
    om->addDrawableObject(wrongSphere);

    printf("  Scene 4: Created 2 spheres for backface test\n");
    printf("    TOP (RED):    Correct Phong shader\n");
    printf("    BOTTOM (GREEN): Wrong Phong shader (abs bug)\n");
}

void Scene4_BackfaceTest::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene4_BackfaceTest::createScene(GLFWwindow* window)
{
    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // ✅ SVETLO ZA GULIČKAMI (osvetľuje prednú stranu, ale kamera vidí zadnú!)
    PointLight* backLight = new PointLight(
        glm::vec3(0.0f, 0.0f, -5.0f),  // ✅ Za guličkami (vzadu v -Z, osvetľuje prednú stranu)
        1.0f,    // constant
        0.0f,    // linear
        0.05f,   // quadratic (slabý útlm)
        glm::vec3(1.0f, 1.0f, 1.0f)   // Biele
    );
    lm->addPointLight(backLight);

    // ✅ NÍZKY ambient, aby bol rozdiel viditeľný
    lm->ambient = 0.05f;
    spm->updateLights();

    printf("  Scene 4: Light at [0, 0, -5] (BEHIND spheres, lights front side)\n");
    printf("  Camera at [0, 0, 10] (FRONT of spheres, sees BACKFACE)\n");
    printf("  Expected:\n");
    printf("    - RED sphere: Front lit, back DARK (correct)\n");
    printf("    - GREEN sphere: BOTH sides lit (wrong!)\n");
}

void Scene4_BackfaceTest::renderFrame()
{
    camera->checkChanges();
    camera->controls();
    om->drawObjects();
}

void Scene4_BackfaceTest::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

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