#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"

class Scene2_SolarSystem : public AbstractScene
{
private:
    float earthAngle = 0.0f;   // Uhol Zeme okolo Slnka
    float moonAngle = 0.0f;    // Uhol Mesiaca okolo Zeme

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};