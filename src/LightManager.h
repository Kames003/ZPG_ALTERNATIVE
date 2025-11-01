#pragma once

#include <vector>
#include <iostream>
#include "Light.h"
#include "PointLight.h"
#include "Spotlight.h"
#include "SpotlightCamera.h"
#include "DirectionalLight.h"

// Manager pre správu všetkých svetiel v scéne
// Každá scéna má vlastnú inštanciu LightManagera
class LightManager
{
private:
    std::vector<PointLight*> pointlights;       // Všetky bodové svetlá
    std::vector<Spotlight*> spotlights;         // Všetky reflektory
    SpotlightCamera* spotlightCamera = nullptr;           // Baterka (max 1)
    DirectionalLight* directional;              // Smerové svetlo (max 1)

public:
    float ambient;  // Globálne ambientné osvetlenie (0.0 - 1.0)

    // Pridanie svetiel
    void addPointLight(PointLight* l);
    void addSpotlight(Spotlight* l);
    void addSpotlightCamera(SpotlightCamera* l);
    void addDirectionalLight(DirectionalLight* l);

    void removeSpotlightCamera()
    {
        spotlightCamera = nullptr;
    }


    // Získanie počtu svetiel
    int getPointLightsSize();
    int getSpotlightsSize();

    // Získanie konkrétneho svetla
    PointLight* getPointLight(int index);
    Spotlight* getSpotlight(int index);
    SpotlightCamera* getSpotlightCamera();
    DirectionalLight* getDirectionalLight();
};