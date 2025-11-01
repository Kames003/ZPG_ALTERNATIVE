#pragma once

#include "SpotlightCamera.h"

// Statický reflektor - má vlastnú pozíciu a smer
// Príklad: svetlomet na javisko, pouličná lampa
class Spotlight : public SpotlightCamera
{
public:
    glm::vec3 position;   // Pozícia reflektora
    glm::vec3 direction;  // Smer svietenia

    Spotlight(glm::vec3 position, glm::vec3 direction,
              float constant, float linear, float quadratic, float angle,
              glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};