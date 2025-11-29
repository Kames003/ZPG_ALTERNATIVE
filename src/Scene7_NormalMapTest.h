// Scene7_NormalMapTest.h - Test normálových map
#pragma once
#include "AbstractScene.h"
#include "LoadedModel.h"

class Scene7_NormalMapTest : public AbstractScene
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
