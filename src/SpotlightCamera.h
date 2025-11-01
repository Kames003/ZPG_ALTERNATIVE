#pragma once

#include "Light.h"

// Baterka pripojená ku kamere - pohybuje sa s hráčom
// Svieti len v úzkom kuželi v smere pohľadu
class SpotlightCamera : public Light
{
public:
    float constant;      // Útlm - konštantný
    float linear;        // Útlm - lineárny
    float quadratic;     // Útlm - kvadratický
    float angle;         // Uhol kužeľa svetla (cosinus!)

    SpotlightCamera(float constant, float linear, float quadratic, float angle,
                    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};