#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"

/**
 * @brief Scéna 4: Backface Culling Test
 * 
 * Testovacia scéna pre overenie správneho spracovania odvrátených strán:
 * - Svetlo ZA guličkami (z=-5, osvetľuje prednú stranu)
 * - Kamera PRED guličkami (z=10, vidí zadnú/odvrátenu stranu)
 * - Zadná (odvrátená) strana by NEMALA byť osvetlená
 *
 * Layout (pohľad zboku, os Z smeruje k nám):
 *
 *     💡                  🔴🟢              👁️
 *   SVETLO             GULIČKY           KAMERA
 *   z=-5               z=0               z=10
 *   (vzadu)         (v strede)          (vpredu)
 *
 *   Svetlo svieti na PREDNÚ stranu (z=+), ale kamera vidí ZADNÚ stranu!
 *
 * Konfigurácia:
 * - Svetlo: [0, 0, -5] (za guličkami, ďaleko vzadu)
 * - Správna gulička (červená): [0, 1, 0] - Phong CORRECT shader
 * - Zlá gulička (zelená): [0, -1, 0] - Phong WRONG shader (abs bug)
 * - Kamera: [0, 0, 10] (default, pred guličkami)
 *
 * Očakávaný výsledok:
 * - ČERVENÁ gulička: TMAVÁ (správne - backface nie je osvetlený)
 * - ZELENÁ gulička: SVETLÁ (chyba - abs() osvetlí aj backface!)
 */
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