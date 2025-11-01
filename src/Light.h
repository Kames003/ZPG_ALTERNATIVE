#pragma once

#include <glm/glm.hpp>

// Základná abstraktná trieda pre všetky typy svetiel
class Light
{
public:
    glm::vec3 color;  // Farba svetla (RGB)
};