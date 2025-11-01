#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"

/**
 * @brief Scéna 2: Slnečná sústava
 * 
 * Animovaná scéna s:
 * - Slnko v strede (žlté, Constant shader)
 * - Zem obieha okolo Slnka (modrá, Phong shader)
 * - Mesiac obieha okolo Zeme (sivý, Lambert shader)
 * 
 * Hierarchická animácia:
 * - Zem: Rotácia okolo Slnka
 * - Mesiac: Rotácia okolo Slnka + Rotácia okolo Zeme
 * 
 * Rýchlosti:
 * - Zem: 1.0° za frame
 * - Mesiac: 3.0° za frame (rýchlejšie)
 */
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