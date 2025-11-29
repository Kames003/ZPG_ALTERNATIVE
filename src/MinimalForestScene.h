#pragma once
#include "AbstractScene.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "ObjectInteractionManager.h"
#include "BezierSpline.h"
#include <vector>


class Firefly;
class SpotlightCamera;
class DrawableObject;


class MinimalForestScene : public AbstractScene
{
private:
    std::vector<Firefly*> fireflies;
    MaterialManager* mm = nullptr;
    ObjectInteractionManager* interactionManager = nullptr;

    SpotlightCamera* flashlight = nullptr;
    bool flashlightEnabled = false;
    bool keyFWasPressed = false;

    // Bezier curve test
    BezierSpline* planetPath = nullptr;
    DrawableObject* orbitingPlanet = nullptr;

protected:
    void createShaders() override;
    void createTextures();
    void createMaterials();
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;

    void handleFlashlightInput();

    // Mouse interaction - deleguje na ObjectInteractionManager
    void handleMouseInput();
    void placeTreeAt(glm::vec3 worldPos);
    void removeObjectAt(int stencilID);

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};