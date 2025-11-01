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
#include "SceneManager.h"

// ✅ Include konkrétnych scén
#include "Scene5_LoadedModelTest.h"
#include "MinimalForestScene.h"
#include "Scene1_PhongTest.h"
#include "Scene2_SolarSystem.h"
#include "Scene3_LightingDemo.h"
#include "Scene4_BackfaceTest.h"

/**
 * @brief Hlavná aplikácia
 *
 * UPDATED: Teraz používa SceneManager pre prepínanie medzi scénami
 */
class Application
{
private:
	SceneManager* sceneManager;
	Window* window;

	void initGLFW();
	void initGLEW();
	void initWindow(int width, int height, const char* name);
	void printVersionInfo();

public:
	/**
	 * @brief Inicializuje scénu (teraz cez SceneManager)
	 */
	void initScene();

	/**
	 * @brief Spustí hlavný loop (teraz cez SceneManager)
	 */
	void run();
};