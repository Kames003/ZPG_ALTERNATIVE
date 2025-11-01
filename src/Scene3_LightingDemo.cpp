#include "Scene3_LightingDemo.h"

void Scene3_LightingDemo::createShaders()
{
    // âœ… LEN PHONG SHADER - pre vÅ¡etky sfÃ©ry
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");
}

void Scene3_LightingDemo::createDrawableObjects()
{
    SphereModel* sphereModel = new SphereModel();
    ShaderProgram* phongShader = spm->getShaderProgram(0);

    // âœ… SpoloÄnÃ¡ farba pre vÅ¡etky sfÃ©ry
    glm::vec3 sphereColor(0.0f, 0.8f, 0.0f);  // ZelenÃ¡ ako na fotke

    // ========================================
    // SFÃ‰RA 1: TOP - KladnÃ¡ Y os
    // ========================================
    DrawableObject* sphere1 = new DrawableObject(
        sphereModel,
        phongShader,
        sphereColor
    );
    sphere1->translate(glm::vec3(0.0f, 0.6f, 0.0f));
    sphere1->scale(glm::vec3(0.25f));
    sphere1->calculateModelMatrix();
    sphere1->updateModelMatrix();
    om->addDrawableObject(sphere1);

    // ========================================
    // SFÃ‰RA 2: RIGHT - KladnÃ¡ X os
    // ========================================
    DrawableObject* sphere2 = new DrawableObject(
        sphereModel,
        phongShader,
        sphereColor
    );
    sphere2->translate(glm::vec3(0.6f, 0.0f, 0.0f));
    sphere2->scale(glm::vec3(0.25f));
    sphere2->calculateModelMatrix();
    sphere2->updateModelMatrix();
    om->addDrawableObject(sphere2);

    // ========================================
    // SFÃ‰RA 3: BOTTOM - ZÃ¡pornÃ¡ Y os
    // ========================================
    DrawableObject* sphere3 = new DrawableObject(
        sphereModel,
        phongShader,
        sphereColor
    );
    sphere3->translate(glm::vec3(0.0f, -0.6f, 0.0f));
    sphere3->scale(glm::vec3(0.25f));
    sphere3->calculateModelMatrix();
    sphere3->updateModelMatrix();
    om->addDrawableObject(sphere3);

    // ========================================
    // SFÃ‰RA 4: LEFT - ZÃ¡pornÃ¡ X os
    // ========================================
    DrawableObject* sphere4 = new DrawableObject(
        sphereModel,
        phongShader,
        sphereColor
    );
    sphere4->translate(glm::vec3(-0.6f, 0.0f, 0.0f));
    sphere4->scale(glm::vec3(0.25f));
    sphere4->calculateModelMatrix();
    sphere4->updateModelMatrix();
    om->addDrawableObject(sphere4);

    printf("  Scene 3: Created 4 green spheres with Phong shader\n");
    printf("    TOP:    [ 0.0,  0.6,  0.0]\n");
    printf("    RIGHT:  [ 0.6,  0.0,  0.0]\n");
    printf("    BOTTOM: [ 0.0, -0.6,  0.0]\n");
    printf("    LEFT:   [-0.6,  0.0,  0.0]\n");
}

void Scene3_LightingDemo::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });
}

void Scene3_LightingDemo::createScene(GLFWwindow* window)
{
    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 100.0f);
    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();

    createShaders();
    createDrawableObjects();
    callbacks();

    // âœ… SVETLO PRESNE V STREDE [0, 0, 0]
    PointLight* centerLight = new PointLight(
        glm::vec3(0.0f, 0.0f, 0.0f),  // âœ… Stred!
        1.0f,    // constant
        0.0f,    // linear (Å¾iadny Ãºtlum)
        0.5f,    // quadratic (mierne Ãºtlm)
        glm::vec3(1.0f, 1.0f, 1.0f)   // Biele
    );
    lm->addPointLight(centerLight);
    lm->ambient = 0.2f;  // âœ… Mierny ambient
    spm->updateLights();

    printf("  Scene 3: Light at [0.0, 0.0, 0.0] (CENTER)\n");
    printf("  Expected: 4 spheres with SYMMETRIC Phong lighting\n");
    printf("\n");
    printf("  ╔════════════════════════════════════════╗\n");
    printf("  ║       FOV TESTING CONTROLS             ║\n");
    printf("  ╠════════════════════════════════════════╣\n");
    printf("  ║  F1  -  FOV = 45°  (narrow/telephoto)  ║\n");
    printf("  ║  F2  -  FOV = 90°  (normal)            ║\n");
    printf("  ║  F3  -  FOV = 130° (wide/fisheye)      ║\n");
    printf("  ╠════════════════════════════════════════╣\n");
    printf("  ║  Camera: W/A/S/D, Space/Shift, Mouse   ║\n");
    printf("  ╚════════════════════════════════════════╝\n");
    printf("\n");
    printf("  Current FOV: %.0f°\n", camera->getFOV());
}

void Scene3_LightingDemo::renderFrame()
{
    camera->checkChanges();
    camera->controls();

    // ✅ NOVÉ: Obsluha FOV zmeny
    handleFOVInput();

    om->drawObjects();
}

// ========================================
// ✅ NOVÁ METÓDA: FOV INPUT HANDLING
// ========================================
void Scene3_LightingDemo::handleFOVInput()
{
    // F1: FOV = 45°
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        if (!keyF1WasPressed)
        {
            camera->setFOV(45.0f);
            currentFOV = 45.0f;
            printf("\n✅ FOV changed to 45° (narrow, telephoto)\n");
            keyF1WasPressed = true;
        }
    }
    else
    {
        keyF1WasPressed = false;
    }

    // F2: FOV = 90°
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (!keyF2WasPressed)
        {
            camera->setFOV(90.0f);
            currentFOV = 90.0f;
            printf("\n✅ FOV changed to 90° (normal)\n");
            keyF2WasPressed = true;
        }
    }
    else
    {
        keyF2WasPressed = false;
    }

    // F3: FOV = 130°
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        if (!keyF3WasPressed)
        {
            camera->setFOV(130.0f);
            currentFOV = 130.0f;
            printf("\n✅ FOV changed to 130° (wide, fisheye)\n");
            keyF3WasPressed = true;
        }
    }
    else
    {
        keyF3WasPressed = false;
    }
}

void Scene3_LightingDemo::renderScene()
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