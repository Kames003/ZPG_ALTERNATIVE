#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"

/**
 * @brief Scéna 3: Phong Lighting Test - Symetrické osvetlenie + FOV test
 *
 * Testovacia scéna pre overenie správnosti Phong osvetľovacieho modelu:
 * - 4 zelené sféry rozložené symetricky na osiach
 * - 1 bodové svetlo PRESNE v strede [0,0,0]
 * - Všetky sféry používajú Phong shader
 * - Všetky sféry by mali byť osvetlené symetricky
 *
 * ✅ PRIDANÉ: FOV Testing (Field of View)
 * - Klávesa F1: FOV = 45° (úzky uhol, "teleobjektív")
 * - Klávesa F2: FOV = 90° (normálny uhol)
 * - Klávesa F3: FOV = 130° (široký uhol, "rybie oko")
 *
 * Layout (pohľad zhora, os Y smeruje hore):
 *
 *           [TOP]
 *             |
 *    [LEFT] - 💡 - [RIGHT]
 *             |
 *         [BOTTOM]
 *
 * Pozície sfér:
 * - TOP:    [ 0.0,  0.6,  0.0]
 * - RIGHT:  [ 0.6,  0.0,  0.0]
 * - BOTTOM: [ 0.0, -0.6,  0.0]
 * - LEFT:   [-0.6,  0.0,  0.0]
 *
 * Svetlo: [0.0, 0.0, 0.0] (stred)
 *
 * Očakávaný výsledok:
 * - Všetky sféry majú rovnaké osvetlenie (symetria)
 * - Viditeľný diffuse aj specular komponent
 * - Jemné odlesky (highlights) na každej sfére
 */
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