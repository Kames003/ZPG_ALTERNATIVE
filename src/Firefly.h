#pragma once

#include "PointLight.h"
#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

/**
 * @brief Světluška = Bodové světlo + Vizuální reprezentace
 *
 * - Obsahuje PointLight (pro osvětlení)
 * - Obsahuje DrawableObject (pro zobrazení)
 *
 * 
 *
 */
class Firefly
{
private:
    PointLight* light;          // Světlo (pro osvětlení scény)
    DrawableObject* visual;     // Vizualizace (malá kulička)

    // Animace
    glm::vec3 centerPosition;   // Střed orbitu
    float orbitRadius;          // Poloměr orbitu
    float orbitSpeed;           // Rychlost pohybu
    float verticalSpeed;        // Rychlost vertikálního pohybu
    float phase;                // Fázový posun (pro různorodost)

    // Vlastnosti
    glm::vec3 color;            // Barva světla
    float intensity;            // Intenzita (pro blikání)
    float minIntensity;         // Minimální intenzita
    float maxIntensity;         // Maximální intenzita
    float blinkSpeed;           // Rychlost blikání

    // ✅ NOVÉ: Uložený scale pro správnou animaci
    glm::vec3 visualScale;      // Scale vizualizace (podle typu světlušky)

public:

    Firefly(glm::vec3 position, 
            AbstractModel* model, 
            ShaderProgram* shader,
            glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.5f));
    

    void updateAnimation(float time);
    void setCenterPosition(glm::vec3 pos);
    void setOrbitRadius(float radius);
    void setSpeed(float speed);
    PointLight* getLight();
    DrawableObject* getVisual();
    void setVisualVisible(bool visible);
};