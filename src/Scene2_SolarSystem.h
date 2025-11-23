#pragma once

#include "AbstractScene.h"
#include "LoadedModel.h"
#include "DrawableObject.h"
#include "TextureManager.h"
#include "Texture2D.h"

/**
 * @brief Kompletná slnečná sústava s realistickým pohybom
 *
 * Demonstrácia Composite pattern pre hierarchické transformácie:
 * - Rotácia okolo vlastnej osi (deň/noc)
 * - Rotácia okolo Slnka (orbit - rok)
 * - Hierarchická závislosť mesiacov na planétach
 *
 * 8 planét: Merkúr, Venuša, Zem, Mars, Jupiter, Saturn, Urán, Neptún
 * Mesiace: Mesiac (Zem), Phobos (Mars)
 */
class Scene2_SolarSystem : public AbstractScene
{
private:
    LoadedModel* planetModel;

    // ========================================
    // UHLY OBEHU (orbit) - rotácia okolo Slnka
    // ========================================
    float mercuryOrbit = 0.0f;
    float venusOrbit = 0.0f;
    float earthOrbit = 0.0f;
    float marsOrbit = 0.0f;
    float jupiterOrbit = 0.0f;
    float saturnOrbit = 0.0f;
    float uranusOrbit = 0.0f;
    float neptuneOrbit = 0.0f;

    // Mesiace
    float moonOrbit = 0.0f;
    float phobosOrbit = 0.0f;

    // ========================================
    // UHLY ROTÁCIE (axis) - rotácia okolo vlastnej osi
    // ========================================
    float sunAxis = 0.0f;
    float mercuryAxis = 0.0f;
    float venusAxis = 0.0f;
    float earthAxis = 0.0f;
    float marsAxis = 0.0f;
    float jupiterAxis = 0.0f;
    float saturnAxis = 0.0f;
    float uranusAxis = 0.0f;
    float neptuneAxis = 0.0f;

    // ========================================
    // RÝCHLOSTI OBEHU (realistické pomery)
    // Merkúr = 1.0 (referencia), ostatné pomalšie
    // ========================================
    const float MERCURY_ORBIT_SPEED = 1.0f;      // 88 dní
    const float VENUS_ORBIT_SPEED = 0.39f;       // 225 dní
    const float EARTH_ORBIT_SPEED = 0.24f;       // 365 dní
    const float MARS_ORBIT_SPEED = 0.13f;        // 687 dní
    // Vonkajšie planéty - zrýchlené pre demonštráciu (stále pomalšie ako Mars)
    const float JUPITER_ORBIT_SPEED = 0.08f;     // zrýchlené pre demo
    const float SATURN_ORBIT_SPEED = 0.05f;      // zrýchlené pre demo
    const float URANUS_ORBIT_SPEED = 0.03f;      // zrýchlené pre demo
    const float NEPTUNE_ORBIT_SPEED = 0.02f;     // zrýchlené pre demo

    const float MOON_ORBIT_SPEED = 3.0f;         // 27 dní okolo Zeme
    const float PHOBOS_ORBIT_SPEED = 10.0f;      // 7.6 hodín okolo Marsu

    // ========================================
    // RÝCHLOSTI ROTÁCIE okolo vlastnej osi
    // ========================================
    const float SUN_AXIS_SPEED = 0.1f;           // 25 dní
    const float MERCURY_AXIS_SPEED = 0.2f;       // zrýchlené pre demo
    const float VENUS_AXIS_SPEED = -0.3f;        // zrýchlené pre demo, RETROGRÁDNA (-)!
    const float EARTH_AXIS_SPEED = 1.0f;         // 24 hodín (referencia)
    const float MARS_AXIS_SPEED = 0.97f;         // 24.6 hodín
    const float JUPITER_AXIS_SPEED = 2.4f;       // 9.9 hodín (najrýchlejšia!)
    const float SATURN_AXIS_SPEED = 2.2f;        // 10.7 hodín
    const float URANUS_AXIS_SPEED = 1.4f;        // 17.2 hodín
    const float NEPTUNE_AXIS_SPEED = 1.5f;       // 16.1 hodín

    // ========================================
    // VZDIALENOSTI OD SLNKA (škálované)
    // ========================================
    const float MERCURY_DIST = 1.2f;
    const float VENUS_DIST = 1.8f;
    const float EARTH_DIST = 2.5f;
    const float MARS_DIST = 3.3f;
    const float JUPITER_DIST = 5.0f;
    const float SATURN_DIST = 6.5f;
    const float URANUS_DIST = 8.0f;
    const float NEPTUNE_DIST = 9.5f;

    const float MOON_DIST = 0.4f;     // Od Zeme
    const float PHOBOS_DIST = 0.25f;  // Od Marsu

    // ========================================
    // VEĽKOSTI (škálované pre viditeľnosť)
    // ========================================
    const float SUN_SIZE = 0.6f;
    const float MERCURY_SIZE = 0.08f;
    const float VENUS_SIZE = 0.15f;
    const float EARTH_SIZE = 0.16f;
    const float MARS_SIZE = 0.12f;
    const float JUPITER_SIZE = 0.4f;    // Najväčšia
    const float SATURN_SIZE = 0.35f;
    const float URANUS_SIZE = 0.22f;
    const float NEPTUNE_SIZE = 0.2f;
    const float MOON_SIZE = 0.05f;
    const float PHOBOS_SIZE = 0.025f;

    // ========================================
    // NÁKLONY OSÍ (stupne)
    // ========================================
    const float EARTH_TILT = 23.5f;
    const float MARS_TILT = 25.2f;
    const float SATURN_TILT = 26.7f;
    const float URANUS_TILT = 97.8f;  // Takmer "na boku"!
    const float NEPTUNE_TILT = 28.3f;

    // Globálny multiplikátor rýchlosti
    const float SPEED_MULT = 20.0f;

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};
