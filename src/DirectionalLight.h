#pragma once

#include "Light.h"

// Smerové svetlo - všetky lúče idú v rovnakom smere
// Príklad: slnko (na scéne je tak ďaleko, že lúče sú paralelné)
class DirectionalLight : public Light
{
public:
    glm::vec3 direction;  // Smer svetla (normalizovaný vektor)

    DirectionalLight(glm::vec3 direction,
                     glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};