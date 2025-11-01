#pragma once

#include "ShaderProgramManager.h"
#include "ObjectManager.h"
#include "LightManager.h"
#include "Callback.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief Abstraktná základná trieda pre všetky scény
 * 
 * Táto trieda definuje štruktúru a rozhranie pre všetky scény v aplikácii.
 * Každá konkrétna scéna musí dediť z tejto triedy a implementovať všetky
 * čisto virtuálne metódy.
 * 
 * Štruktúra scény:
 * 1. createScene() - inicializácia (vytvorenie managerov, kamery)
 * 2. createShaders() - pridanie všetkých shader programov
 * 3. createLights() - vytvorenie a konfigurácia svetiel (ak je potrebné)
 * 4. createDrawableObjects() - vytvorenie všetkých objektov scény
 * 5. callbacks() - nastavenie callback funkcií pre vstupy
 * 6. renderScene() - hlavný render loop
 */
class AbstractScene
{
protected:
	// ========================================
	// CORE KOMPONENTY SCÉNY
	// ========================================
	
	/**
	 * @brief Kamera scény - každá scéna má vlastnú kameru
	 * 
	 * Výhody:
	 * - Každá scéna môže mať inú kameru (statická/dynamická)
	 * - Iné FOV, near/far plane
	 * - Iná počiatočná pozícia
	 */
	Camera* camera;
	
	/**
	 * @brief Manager pre shader programy
	 * 
	 * Správa všetkých shader programov scény:
	 * - Pridávanie shader programov
	 * - Získavanie shader programov podľa indexu
	 * - Update svetiel vo všetkých shaderoch
	 */
	ShaderProgramManager* spm;
	
	/**
	 * @brief Manager pre vykresľované objekty
	 * 
	 * Správa všetkých DrawableObject v scéne:
	 * - Pridávanie objektov
	 * - Vykreslenie všetkých objektov
	 * - Správa skyboxu
	 */
	ObjectManager* om;
	
	/**
	 * @brief Manager pre svetlá
	 * 
	 * Správa všetkých svetiel v scéne:
	 * - PointLight (bodové svetlá)
	 * - Spotlight (reflektory)
	 * - SpotlightCamera (baterka)
	 * - DirectionalLight (smerové svetlo)
	 */
	LightManager* lm;
	
	/**
	 * @brief Manager pre textúry
	 * 
	 * Správa všetkých textúr v scéne:
	 * - 2D textúry
	 * - Cubemap textúry (pre skybox
	
	/**
	 * @brief GLFW okno
	 * 
	 * Referencia na hlavné okno aplikácie
	 */
	GLFWwindow* window;

	// ========================================
	// ABSTRAKTNÉ METÓDY - MUSIA BYŤ IMPLEMENTOVANÉ
	// ========================================
	
	/**
	 * @brief Vytvorí všetky shader programy pre scénu
	 * 
	 * Táto metóda musí:
	 * - Načítať všetky potrebné shadery (vertex + fragment)
	 * - Pridať ich do ShaderProgramManager-a pomocou addShaderProgram()
	 * - Shader programy sa indexujú od 0 (v poradí pridávania)
	 * 
	 * Príklad:
	 * @code
	 * void Scene::createShaders() {
	 *     spm->addShaderProgram(camera, "vertex.glsl", "fragment.glsl");  // index 0
	 *     spm->addShaderProgram(camera, "vertex2.glsl", "fragment2.glsl"); // index 1
	 * }
	 * @endcode
	 */
	virtual void createShaders() = 0;
	
	/**
	 * @brief Vytvorí všetky vykresľované objekty scény
	 * 
	 * Táto metóda musí:
	 * - Vytvoriť všetky modely (sphere, tree, loaded models, atď.)
	 * - Vytvoriť DrawableObject pre každý model
	 * - Nastaviť transformácie (translate, rotate, scale)
	 * - Pridať objekty do ObjectManager-a pomocou addDrawableObject()
	 * 
	 * Príklad:
	 * @code
	 * void Scene::createDrawableObjects() {
	 *     DrawableObject* obj = new DrawableObject(model, shaderProgram);
	 *     obj->translate(glm::vec3(1, 0, 0));
	 *     obj->calculateModelMatrix();
	 *     om->addDrawableObject(obj);
	 * }
	 * @endcode
	 */
	virtual void createDrawableObjects() = 0;
	
	/**
	 * @brief Nastaví callback funkcie pre vstupy
	 * 
	 * Táto metóda musí:
	 * - Nastaviť callback pre myš (glfwSetCursorPosCallback)
	 * - Nastaviť callback pre tlačidlá myši (glfwSetMouseButtonCallback)
	 * - Nastaviť callback pre zmenu veľkosti okna (glfwSetWindowSizeCallback)
	 * - Nastaviť callback pre klávesy (ak potrebné)
	 * 
	 * Príklad:
	 * @code
	 * void Scene::callbacks() {
	 *     glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
	 *         Callback::GetInstance().cursorCallback(w, x, y);
	 *     });
	 * }
	 * @endcode
	 */
	virtual void callbacks() = 0;

public:
	// ========================================
	// VEREJNÉ VIRTUÁLNE METÓDY
	// ========================================
	
	/**
	 * @brief Vytvorí a inicializuje celú scénu
	 * 
	 * Táto metóda musí:
	 * 1. Uložiť si referenciu na okno (this->window = window)
	 * 2. Vytvoriť kameru (this->camera = new Camera(...))
	 * 3. Vytvoriť všetkých managerov (lm, spm, om, tm)
	 * 4. Zavolať createShaders()
	 * 5. Zavolať createLights() (ak existuje)
	 * 6. Zavolať createDrawableObjects()
	 * 7. Zavolať callbacks()
	 * 
	 * @param window GLFW okno aplikácie
	 * 
	 * Príklad implementácie:
	 * @code
	 * void Scene::createScene(GLFWwindow* window) {
	 *     this->window = window;
	 *     this->camera = new Camera(window, 45.0f, 0.1f, 100.0f);
	 *     this->lm = new LightManager();
	 *     this->spm = new ShaderProgramManager(lm);
	 *     this->om = new ObjectManager();
	 *     this->tm = new TextureManager();
	 *     
	 *     createShaders();
	 *     createLights();
	 *     createDrawableObjects();
	 *     callbacks();
	 * }
	 * @endcode
	 */
	virtual void createScene(GLFWwindow* window) = 0;
	
	/**
	 * @brief Hlavný render loop scény
	 * 
	 * Táto metóda musí:
	 * 1. Povoliť potrebné OpenGL funkcie (depth test, stencil test, atď.)
	 * 2. Spustiť hlavný loop (while (!glfwWindowShouldClose(window)))
	 * 3. Vymazať buffery (glClear)
	 * 4. Updatovať kameru (camera->checkChanges(), camera->controls())
	 * 5. Vykresliť skybox (ak existuje)
	 * 6. Vykresliť všetky objekty (om->drawObjects())
	 * 7. Spracovať animácie (ak existujú)
	 * 8. Spracovať vstupy (glfwPollEvents)
	 * 9. Swapnúť buffery (glfwSwapBuffers)
	 * 10. Po skončení zrušiť okno a GLFW
	 * 
	 * Príklad implementácie:
	 * @code
	 * void Scene::renderScene() {
	 *     glEnable(GL_DEPTH_TEST);
	 *     
	 *     while (!glfwWindowShouldClose(window)) {
	 *         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 *         
	 *         camera->checkChanges();
	 *         camera->controls();
	 *         
	 *         om->drawObjects();
	 *         
	 *         glfwPollEvents();
	 *         glfwSwapBuffers(window);
	 *     }
	 *     
	 *     glfwDestroyWindow(window);
	 *     glfwTerminate();
	 *     exit(EXIT_SUCCESS);
	 * }
	 * @endcode
	 */
	virtual void renderScene() = 0;
	
	/**
	 * @brief Virtuálny destruktor
	 * 
	 * Umožňuje správne mazanie objektov cez pointer na AbstractScene
	 */

};