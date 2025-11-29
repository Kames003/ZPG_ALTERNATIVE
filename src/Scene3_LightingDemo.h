#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"


class Scene3_LightingDemo : public AbstractScene
{
private:
    // ✅ FOV test - debouncing pre klávesy
    bool keyF1WasPressed = false;
    bool keyF2WasPressed = false;
    bool keyF3WasPressed = false;

    // Aktuálny FOV
    float currentFOV = 60.0f;

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

    // ✅ NOVÉ: Obsluha FOV klávesov
    void handleFOVInput();

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};