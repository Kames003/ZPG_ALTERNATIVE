#include "SpotlightCamera.h"

SpotlightCamera::SpotlightCamera(float constant, float linear, float quadratic,
                                  float angle, glm::vec3 color)
{
    this->color = color;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->angle = angle;  // Používa sa cos(angle), napr. cos(12.5°) = 0.98
}