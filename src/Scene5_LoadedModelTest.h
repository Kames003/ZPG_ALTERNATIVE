// Scene5_LoadedModelTest.h
#pragma once
#include "AbstractScene.h"
#include "LoadedModel.h"

class Scene5_LoadedModelTest : public AbstractScene
{
protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};