#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Callback.h"
#include "AbstractScene.h"

// Include konkrétnych scén
#include "MinimalForestScene.h"
// #include "Scene7_ForestWithFireflies.h"  // Ak budete chcieť použiť

/**
 * @brief Hlavná aplikácia
 *
 * Application je centrálny bod celej aplikácie.
 * Stará sa o:
 * - Inicializáciu GLFW a GLEW
 * - Vytvorenie okna
 * - Výber a vytvorenie scény
 * - Spustenie render loop-u
 *
 * Workflow:
 * 1. Vytvor Application
 * 2. Zavolaj initScene() - inicializuje všetko
 * 3. Zavolaj run() - spustí render loop
 *
 * Príklad v main():
 * @code
 * Application* app = new Application();
 * app->initScene();
 * app->run();
 * @endcode
 */
class Application
{
private:
	/**
	 * @brief Aktívna scéna
	 *
	 * Pointer na AbstractScene - môže byť Scene, SphereScene, atď.
	 * Toto je scéna ktorá sa vykreslí po zavolaní run()
	 */
	AbstractScene* as;

	/**
	 * @brief Okno aplikácie
	 *
	 * Wrapper pre GLFW okno
	 */
	Window* window;

	/**
	 * @brief Inicializuje GLFW
	 *
	 * Zavolá glfwInit() a nastaví verziu OpenGL.
	 * Ak zlyhá, aplikácia sa ukončí.
	 */
	void initGLFW();

	/**
	 * @brief Inicializuje GLEW
	 *
	 * Zavolá glewInit() pre načítanie OpenGL funkcií.
	 */
	void initGLEW();

	/**
	 * @brief Vytvorí okno
	 *
	 * @param width Šírka okna v pixeloch
	 * @param height Výška okna v pixeloch
	 * @param name Názov okna
	 */
	void initWindow(int width, int height, const char* name);

	/**
	 * @brief Vypíše informácie o OpenGL
	 *
	 * Vypíše:
	 * - Verziu OpenGL
	 * - Verziu GLEW
	 * - Verziu GLFW
	 * - Vendor (výrobca GPU)
	 * - Renderer (model GPU)
	 * - Verziu GLSL (shader language)
	 */
	void printVersionInfo();

public:
	/**
	 * @brief Inicializuje scénu
	 *
	 * Táto metóda:
	 * 1. Inicializuje GLFW
	 * 2. Vytvorí okno
	 * 3. Inicializuje GLEW
	 * 4. Vypíše informácie o OpenGL
	 * 5. Vytvorí scénu (Scene alebo SphereScene)
	 * 6. Zavolá scéna->createScene()
	 *
	 * DÔLEŽITÉ: Tu vyberiete ktorú scénu chcete použiť!
	 * Jednoducho odkomentujte riadok s požadovanou scénou.
	 *
	 * Príklad:
	 * @code
	 * void Application::initScene() {
	 *     // ...inicializácia...
	 *
	 *     // Vyberte scénu:
	 *     as = new Scene();              // Hlavná lesná scéna
	 *     // as = new SphereScene();     // Testovacia scéna so sférami
	 *     // as = new Scene7_ForestWithFireflies();  // Scéna pre cvičenie 7
	 *
	 *     as->createScene(window->getWindow());
	 * }
	 * @endcode
	 */
	void initScene();

	/**
	 * @brief Spustí render loop
	 *
	 * Zavolá scéna->renderScene(), ktorá obsahuje hlavný render loop.
	 * Táto metóda sa vráti až keď používateľ zatvorí okno.
	 */
	void run();
};