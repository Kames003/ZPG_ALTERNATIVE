#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
{
    this->color = color;
    this->direction = direction;
}