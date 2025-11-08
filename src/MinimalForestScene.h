#pragma once
#include "AbstractScene.h"
#include "TextureManager.h"
#include <vector>

// Forward declaration
class Firefly;
class SpotlightCamera;


class MinimalForestScene : public AbstractScene
{
private:
    std::vector<Firefly*> fireflies;  // Uložení světlušek pro animaci


    SpotlightCamera* flashlight = nullptr;
    bool flashlightEnabled = false;
    bool keyFWasPressed = false;

protected:
    void createShaders() override;
    void createTextures();  // NOVÉ: vytvorenie textúr
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;


    void handleFlashlightInput();

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};