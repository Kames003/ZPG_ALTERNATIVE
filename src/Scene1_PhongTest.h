#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"
#include "TriangleModel.h"


class Scene1_PhongTest : public AbstractScene
{
private:
    float rotationAngle = 0.0f;

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};