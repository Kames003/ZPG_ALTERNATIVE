#include "Application.h"

// ========================================
// TIETO METÓDY ZOSTÁVAJÚ BEZ ZMENY
// ========================================

void Application::initGLFW()
{
    printf("\n=== Initializing GLFW ===\n");

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: Could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("  GLFW initialized successfully\n");
}

void Application::initWindow(int width, int height, const char* name)
{
    printf("\n=== Creating Window ===\n");

    this->window = new Window(width, height, name);
    glfwMakeContextCurrent(this->window->getWindow());
    glfwSwapInterval(1);

    printf("  Window is now current OpenGL context\n");
    printf("  V-Sync enabled\n");
}

void Application::initGLEW()
{
    printf("\n=== Initializing GLEW ===\n");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "ERROR: GLEW initialization failed: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("  GLEW initialized successfully\n");
}

void Application::printVersionInfo()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    OPENGL INFORMATION                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ OpenGL Version:  %-43s ║\n", glGetString(GL_VERSION));
    printf("║ GLSL Version:    %-43s ║\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("║ Vendor:          %-43s ║\n", glGetString(GL_VENDOR));
    printf("║ Renderer:        %-43s ║\n", glGetString(GL_RENDERER));

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("║ GLFW Version:    %d.%d.%-38d ║\n", major, minor, revision);
    printf("║ GLEW Version:    %-43s ║\n", glewGetString(GLEW_VERSION));
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

// ========================================
// ✅ OPRAVENÉ METÓDY
// ========================================

void Application::initScene()
{
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           ZPG - OPENGL PROJECT INITIALIZATION                ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");

    initGLFW();
    initWindow(800, 600, "ZPG - OpenGL Project");
    initGLEW();
    printVersionInfo();

    // ✅ Vytvorte SceneManager
    sceneManager = new SceneManager(this->window->getWindow());

    // ✅ ZAREGISTRUJTE VŠETKY SCÉNY
    sceneManager->registerScene(new MinimalForestScene(), "1. Minimal Forest");
    sceneManager->registerScene(new Scene1_PhongTest(), "2. Minimal Forest");
    sceneManager->registerScene(new Scene2_SolarSystem(), "3. Minimal Forest");
    sceneManager->registerScene(new Scene3_LightingDemo(), "4. Minimal Forest");
    sceneManager->registerScene(new Scene4_BackfaceTest(), "5. Backface Test");

   // sceneManager->registerScene(new Scene1_RotatingTriangle(), "2. Rotating Triangle");
    // sceneManager->registerScene(new Scene2_FourSpheres(), "3. Four Spheres");

    // Nastavíme prvú scénu ako aktívnu
    sceneManager->setActiveScene(0);

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           INITIALIZATION COMPLETE - READY TO RENDER          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ Press H     - Show scene list                                ║\n");
    printf("║ Press 1-3   - Switch scenes                                  ║\n");
    printf("║ Press ESC   - Exit                                           ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
}

void Application::run()
{
    printf("=== Starting Main Loop ===\n\n");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Hlavný render loop
    while (!glfwWindowShouldClose(this->window->getWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // SceneManager vykreslí aktívnu scénu
        sceneManager->renderActiveScene();

        glfwSwapBuffers(this->window->getWindow());
        glfwPollEvents();
    }

    // ========================================
    // CLEANUP
    // ========================================
    printf("\n=== Shutting down ===\n");

    delete sceneManager;

    glfwDestroyWindow(this->window->getWindow());
    glfwTerminate();

    printf("✅ Application terminated successfully.\n");
    exit(EXIT_SUCCESS);
}