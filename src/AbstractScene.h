#pragma once

#include "ShaderProgramManager.h"
#include "ObjectManager.h"
#include "LightManager.h"
#include "Callback.h"

class AbstractScene
{
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
    virtual void createScene(GLFWwindow* window) = 0;
    virtual void renderScene() = 0;
};

