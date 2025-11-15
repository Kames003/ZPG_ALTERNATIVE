#pragma once
#include "AbstractScene.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <vector>


class Firefly;
class SpotlightCamera;


class MinimalForestScene : public AbstractScene
{
private:
    std::vector<Firefly*> fireflies;
    MaterialManager* mm = nullptr;

    SpotlightCamera* flashlight = nullptr;
    bool flashlightEnabled = false;
    bool keyFWasPressed = false;

protected:
    void createShaders() override;
    void createTextures();
    void createMaterials();
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;


    void handleFlashlightInput();

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};