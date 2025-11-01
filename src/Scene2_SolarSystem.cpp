#include "Scene2_SolarSystem.h"

void Scene2_SolarSystem::createShaders()
{
    // 3 shadery pre rôzne objekty
    spm->addShaderProgram(camera, 
        "Shaders/VertexShaderConstant.glsl",
        "Shaders/FragmentShaderConstant.glsl");     // 0 - Constant (Slnko)
    
    spm->addShaderProgram(camera, 
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");     // 1 - Phong (Zem)
    
    spm->addShaderProgram(camera, 
        "Shaders/VertexShaderLambert.glsl", 
        "Shaders/FragmentShaderLambert.glsl");      // 2 - Lambert (Mesiac)
}

void Scene2_SolarSystem::createDrawableObjects()
{
    SphereModel* sphereModel = new SphereModel();

    // ========================================
    // SLNKO - v strede, žlté, Constant shader
    // ========================================
    DrawableObject* sun = new DrawableObject(
        sphereModel,
        spm->getShaderProgram(0),  // Constant
        glm::vec3(1.0f, 0.9f, 0.0f)  // Žltá
    );
    sun->translate(glm::vec3(0.0f, 0.0f, 0.0f));
    sun->scale(glm::vec3(0.3f, 0.3f, 0.3f));
    sun->calculateModelMatrix();
    sun->updateModelMatrix();
    om->addDrawableObject(sun);

    // ========================================
    // ZEM - obieha okolo Slnka, modrá, Phong shader
    // ========================================
    DrawableObject* earth = new DrawableObject(
        sphereModel,
        spm->getShaderProgram(1),  // Phong
        glm::vec3(0.2f, 0.4f, 0.8f)  // Modrá
    );
    // Inicializácia (budeme updatovať každý frame)
    earth->translate(glm::vec3(1.2f, 0.0f, 0.0f));
    earth->scale(glm::vec3(0.15f, 0.15f, 0.15f));
    earth->calculateModelMatrix();
    earth->updateModelMatrix();
    om->addDrawableObject(earth);

    // ========================================
    // MESIAC - obieha okolo Zeme, sivý, Lambert shader
    // ========================================
    DrawableObject* moon = new DrawableObject(
        sphereModel,
        spm->getShaderProgram(2),  // Lambert
        glm::vec3(0.7f, 0.7f, 0.7f)  // Sivá
    );
    // Inicializácia (hierarchická transformácia každý frame)
    moon->translate(glm::vec3(1.6f, 0.0f, 0.0f));
    moon->scale(glm::vec3(0.05f, 0.05f, 0.05f));
    moon->calculateModelMatrix();
    moon->updateModelMatrix();
    om->addDrawableObject(moon);

    printf("  Scene 2: Created Solar System\n");
    printf("    Sun:   Yellow, Constant shader\n");
    printf("    Earth: Blue, Phong shader\n");
    printf("    Moon:  Gray, Lambert shader\n");
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

    createShaders();
    createDrawableObjects();
    callbacks();

    // Svetlo v strede (Slnko)
    PointLight* sunLight = new PointLight(
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f, 0.0f, 0.5f,
        glm::vec3(1.0f, 0.9f, 0.7f)  // Teplá žltá
    );
    lm->addPointLight(sunLight);
    lm->ambient = 0.2f;
    spm->updateLights();
}

void Scene2_SolarSystem::renderFrame()
{
    camera->checkChanges();
    camera->controls();

    // ========================================
    // ANIMÁCIA - HIERARCHICKÁ TRANSFORMÁCIA
    // ========================================
    
    // Uhly rotácie (postupne sa zvyšujú)
    earthAngle += 0.4f;
    moonAngle += 0.9f;  // Mesiac sa točí 3x rýchlejšie
    
    if (earthAngle > 360.0f) earthAngle = 0.0f;
    if (moonAngle > 360.0f) moonAngle = 0.0f;

    // ZEM - obieha okolo Slnka
    DrawableObject* earth = om->getDrawableObject(1);
    earth->rotate(earthAngle, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotácia okolo osi Y
    earth->translate(glm::vec3(1.2f, 0.0f, 0.0f));           // Vzdialenosť od Slnka
    earth->scale(glm::vec3(0.15f, 0.15f, 0.15f));
    earth->calculateModelMatrix();
    earth->updateModelMatrix();

    // MESIAC - obieha okolo Zeme (hierarchická transformácia!)
    DrawableObject* moon = om->getDrawableObject(2);
    
    // 1. Najprv aplikujeme rotáciu Zeme (aby Mesiac obiehal so Zemou)
    moon->rotate(earthAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    moon->translate(glm::vec3(1.2f, 0.0f, 0.0f));  // Posuň na pozíciu Zeme
    
    // 2. Potom aplikujeme vlastnú rotáciu Mesiaca okolo Zeme
    moon->rotate(moonAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    moon->translate(glm::vec3(0.4f, 0.0f, 0.0f));  // Vzdialenosť od Zeme
    moon->scale(glm::vec3(0.05f, 0.05f, 0.05f));
    
    moon->calculateModelMatrix();
    moon->updateModelMatrix();

    // Vykresli všetko
    om->drawObjects();
}

void Scene2_SolarSystem::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);  // Tmavo modrá (vesmír)

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