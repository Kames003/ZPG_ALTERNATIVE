#pragma once

#include "ShaderProgramManager.h"
#include "ObjectManager.h"
#include "LightManager.h"
#include "Callback.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Forward declaration
class SceneManager;

/**
 * @brief Abstraktná základná trieda pre všetky scény
 *
 * UPDATED: Teraz podporuje SceneManager
 * - Pridané: renderFrame() - vykreslí jeden frame
 * - Upravené: renderScene() - deprecated, ale funguje
 */
class AbstractScene
{
	// SceneManager potrebuje prístup k protected členom
	friend class SceneManager;

protected:
	Camera* camera;
	ShaderProgramManager* spm;
	ObjectManager* om;
	LightManager* lm;
	GLFWwindow* window;

	virtual void createShaders() = 0;
	virtual void createDrawableObjects() = 0;
	virtual void callbacks() = 0;

public:
	/**
	 * @brief Vytvorí a inicializuje celú scénu
	 */
	virtual void createScene(GLFWwindow* window) = 0;

	/**
	 * @brief Vykreslí JEDEN frame scény (pre SceneManager)
	 *
	 * ✅ NOVÉ: Táto metóda NEMÁ while loop!
	 */
	virtual void renderFrame() = 0;

	/**
	 * @brief Starý spôsob renderovania (s while loopom)
	 *
	 * ⚠️ DEPRECATED: Použite SceneManager namiesto tohto!
	 */
	virtual void renderScene() = 0;

	/**
	 * @brief Virtuálny destruktor
	 */
	virtual ~AbstractScene() = default;
};