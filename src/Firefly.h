#pragma once

#include "PointLight.h"
#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

/**
 * @brief Světluška = Bodové světlo + Vizuální reprezentace
 * 
 * Hybridní přístup:
 * - Obsahuje PointLight (pro osvětlení)
 * - Obsahuje DrawableObject (pro zobrazení)
 * - Automaticky synchronizuje pozice
 * 
 * ✅ NOVÉ: Variace typů světlušek
 * - SILNÁ: constant=1.0, linear=0.5, quadratic=1.2 (dosvit ~3-4m)
 * - NORMÁLNÍ: constant=1.0, linear=0.7, quadratic=1.8 (dosvit ~2-3m)
 * - SLABÁ: constant=1.0, linear=0.9, quadratic=2.5 (dosvit ~1-2m)
 *
 * Parametry osvětlení (noční scéna):
 * - Barva: teplá žlutá/zelená
 * - Vizualizace: malá bílá kulička (0.06-0.10m podle typu)
 *
 * Použití:
 * @code
 * Firefly* firefly = new Firefly(
 *     glm::vec3(5, 2, 0),           // Startovní pozice
 *     sphereModel,                   // Model (malá kulička)
 *     constantShader,                // Shader (bílá konstanta)
 *     glm::vec3(1.0f, 0.9f, 0.6f)   // Teplá žlutá barva
 * );
 *
 * // Přidání do managerů
 * lm->addPointLight(firefly->getLight());
 * om->addDrawableObject(firefly->getVisual());
 *
 * // Každý frame
 * firefly->updateAnimation(glfwGetTime());
 * @endcode
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
    /**
     * @brief Konstruktor světlušky
     *
     * @param position Počáteční pozice
     * @param model Model pro vizualizaci (typicky SphereModel)
     * @param shader Shader pro vizualizaci (typicky Constant - bílá)
     * @param lightColor Barva světla (doporučeno: teplá žlutá/zelená)
     *
     * ✅ AUTOMATICKY vybere náhodný typ světlušky (SILNÁ/NORMÁLNÍ/SLABÁ)
     */
    Firefly(glm::vec3 position, 
            AbstractModel* model, 
            ShaderProgram* shader,
            glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.5f));
    
    /**
     * @brief Aktualizuje animaci světlušky
     * 
     * @param time Aktuální čas (glfwGetTime())
     * 
     * Animace:
     * - Kruhový pohyb v XZ rovině
     * - Sinusový pohyb ve výšce (Y)
     * - Blikání (změna intenzity)
     */
    void updateAnimation(float time);
    
    /**
     * @brief Nastaví střed orbitu
     */
    void setCenterPosition(glm::vec3 pos);
    
    /**
     * @brief Nastaví poloměr orbitu
     */
    void setOrbitRadius(float radius);
    
    /**
     * @brief Nastaví rychlost pohybu
     */
    void setSpeed(float speed);
    
    /**
     * @brief Získá pointer na světlo (pro LightManager)
     */
    PointLight* getLight();
    
    /**
     * @brief Získá pointer na vizualizaci (pro ObjectManager)
     */
    DrawableObject* getVisual();
    
    /**
     * @brief Zapne/vypne vizualizaci
     */
    void setVisualVisible(bool visible);
};