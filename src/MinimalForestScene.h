#pragma once
#include "AbstractScene.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <vector>

// Forward declaration
class Firefly;
class SpotlightCamera;


class MinimalForestScene : public AbstractScene
{
private:
    std::vector<Firefly*> fireflies;  // Uložení světlušek pro animaci
    MaterialManager* mm = nullptr;     // Material manager

    SpotlightCamera* flashlight = nullptr;
    bool flashlightEnabled = false;
    bool keyFWasPressed = false;

protected:
    void createShaders() override;
    void createTextures();  // Vytvorenie textúr
    void createMaterials(); // Vytvorenie materiálov
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;


    void handleFlashlightInput();

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};