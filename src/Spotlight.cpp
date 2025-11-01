#include "Spotlight.h"

Spotlight::Spotlight(glm::vec3 position, glm::vec3 direction,
                     float constant, float linear, float quadratic, float angle,
                     glm::vec3 color)
    : SpotlightCamera(constant, linear, quadratic, angle, color)
{
    this->position = position;
    this->direction = direction;
    this->color = color;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->angle = angle;
}