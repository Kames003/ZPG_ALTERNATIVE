#pragma once

#include "Light.h"

// Bodové svetlo - svieti do všetkých smerov z jedného bodu
// Príklad: žiarovka, svieča
class PointLight : public Light
{
public:
    glm::vec3 position;  // Pozícia svetla v 3D priestore

    // Attenuation faktory (útlm svetla so vzdialenosťou)
    float constant;      // Konštantný faktor
    float linear;        // Lineárny faktor
    float quadratic;     // Kvadratický faktor

    // Konštruktor
    // constant=1.0, linear=0.0, quadratic=0.5 = stredný útlm
    PointLight(glm::vec3 position, float constant, float linear, float quadratic,
               glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};