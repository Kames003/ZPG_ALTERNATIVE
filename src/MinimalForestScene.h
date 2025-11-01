#pragma once
#include "AbstractScene.h"
#include <vector>

// Forward declaration
class Firefly;
class SpotlightCamera;

/**
 * @brief Minimální scéna s lesem a světluškami
 *
 * Noční scéna s:
 * - Tmavým lesem (stromy, lavičky, křoví)
 * - Slabým měsíčním světlem (ambient 0.20)
 * - 12 animovanými světluškami s různými barvami
 * - 🔦 BATERKA (SpotlightCamera) - zapnutie/vypnutie klávesou F
 *
 * Světlušky:
 * - Barvy: teplá žlutá, zelenkavá, bělavá
 * - Animace: kruhový pohyb + vertikální oscilace + blikání
 * - Útlum: dosvit ~2-3 metry
 * - Vizualizace: malé bílé kuličky
 *
 * Ovládání:
 * - F: Zapnout/vypnout baterku
 * - W/A/S/D: Pohyb
 * - Pravé tlačítko myši: Rozhlížení
 */
class MinimalForestScene : public AbstractScene
{
private:
    std::vector<Firefly*> fireflies;  // Uložení světlušek pro animaci

    // 🔦 BATERKA (Flashlight)
    SpotlightCamera* flashlight = nullptr;
    bool flashlightEnabled = false;
    bool keyFWasPressed = false;

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;

    // 🔦 NOVÉ METÓDY
    void handleFlashlightInput();

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};