#include "Application.h"

void Application::initGLFW()
{
	printf("\n=== Initializing GLFW ===\n");

	// Inicializácia GLFW
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	// Nastavenie verzie OpenGL (3.3 Core Profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	printf("  GLFW initialized successfully\n");
}

void Application::initWindow(int width, int height, const char* name)
{
	printf("\n=== Creating Window ===\n");

	// Vytvorenie okna pomocou Window wrapperu
	this->window = new Window(width, height, name);

	// Nastavenie okna ako aktívneho OpenGL contextu
	glfwMakeContextCurrent(this->window->getWindow());

	// Nastavenie V-Sync (synchronizácia s monitorom)
	glfwSwapInterval(1);

	printf("  Window is now current OpenGL context\n");
	printf("  V-Sync enabled\n");
}

void Application::initGLEW()
{
	printf("\n=== Initializing GLEW ===\n");

	// Povolenie experimentálnych funkcií GLEW
	glewExperimental = GL_TRUE;

	// Inicializácia GLEW
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
	printf("╔════════════════════════════════════════════════════════════════╗\n");
	printf("║                    OPENGL INFORMATION                          ║\n");
	printf("╠════════════════════════════════════════════════════════════════╣\n");
	printf("║ OpenGL Version:  %-45s ║\n", glGetString(GL_VERSION));
	printf("║ GLSL Version:    %-45s ║\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("║ Vendor:          %-45s ║\n", glGetString(GL_VENDOR));
	printf("║ Renderer:        %-45s ║\n", glGetString(GL_RENDERER));

	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("║ GLFW Version:    %-45s ║\n", (std::string(std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision))).c_str());
	printf("║ GLEW Version:    %-45s ║\n", glewGetString(GLEW_VERSION));
	printf("╚════════════════════════════════════════════════════════════════╝\n");
	printf("\n");
}

void Application::initScene()
{
	printf("\n");
	printf("╔════════════════════════════════════════════════════════════════╗\n");
	printf("║                ZPG - OPENGL PROJECT INITIALIZATION             ║\n");
	printf("╚════════════════════════════════════════════════════════════════╝\n");

	// 1. Inicializácia GLFW
	initGLFW();

	// 2. Vytvorenie okna
	initWindow(800, 600, "ZPG - OpenGL Project");

	// 3. Inicializácia GLEW
	initGLEW();

	// 4. Vypísanie informácií o OpenGL
	printVersionInfo();

	// 5. Vytvorenie scény
	printf("=== Creating Scene ===\n");

	// ========================================
	// VÝBER SCÉNY - ODKOMENTUJTE POŽADOVANÚ SCÉNU
	// ========================================

	// Hlavná scéna (Scene.cpp) - Vaša pôvodná scéna s lesom
	// as = new Scene();

	// Testovacia scéna so sférami (SphereScene.cpp)
	as = new MinimalForestScene();

	// Scéna pre cvičenie 7 s animovanými svetluškami
	// as = new Scene7_ForestWithFireflies();

	printf("  Selected scene: SphereScene\n");
	printf("  Creating scene...\n");

	// 6. Inicializácia scény
	as->createScene(this->window->getWindow());

	printf("\n");
	printf("╔════════════════════════════════════════════════════════════════╗\n");
	printf("║           INITIALIZATION COMPLETE - READY TO RENDER            ║\n");
	printf("╚════════════════════════════════════════════════════════════════╝\n");
	printf("\n");
}

void Application::run()
{
	printf("=== Starting Render Loop ===\n\n");

	// Spustenie render loop-u scény
	// Táto metóda sa vráti až keď používateľ zatvorí okno
	as->renderScene();
}