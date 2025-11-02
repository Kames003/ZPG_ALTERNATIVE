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
 * @brief Abstraktná base classa pre všetky konkrétne scény ktore z nej maju dediť
 *
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

	// Vytvorí a inicializuje celú scénu
	virtual void createScene(GLFWwindow* window) = 0;
	virtual void renderFrame() = 0;
	virtual void renderScene() = 0;

	virtual ~AbstractScene() = default;
};