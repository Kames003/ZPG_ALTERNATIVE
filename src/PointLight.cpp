#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, float constant, float linear,
                       float quadratic, glm::vec3 color)
{
    this->color = color;
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}