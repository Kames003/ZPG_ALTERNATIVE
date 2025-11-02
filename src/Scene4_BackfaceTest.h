#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"


class Scene4_BackfaceTest : public AbstractScene
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