#pragma once

#include "ShaderProgramManager.h"
#include "ObjectManager.h"
#include "LightManager.h"
#include "Callback.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class AbstractScene
{
protected:
	// ========================================
	// CORE KOMPONENTY SCÉNY
	// ========================================

	Camera* camera;
	ShaderProgramManager* spm;
	ObjectManager* om;
	LightManager* lm;
	GLFWwindow* window;

	// ========================================
	// ABSTRAKTNÉ METÓDY
	// ========================================

	virtual void createShaders() = 0;
	virtual void createDrawableObjects() = 0;
	virtual void callbacks() = 0;

public:
	// ========================================
	// VEREJNÉ VIRTUÁLNE METÓDY
	// ========================================

	virtual void createScene(GLFWwindow* window) = 0;
	virtual void renderScene() = 0;

};