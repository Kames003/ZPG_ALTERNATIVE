#pragma once

#include "TransformationComponent.h"


class LeafRotate : public TransformationComponent
{
public:
    LeafRotate(float angle, glm::vec3 axis);
    glm::mat4 resultMatrix();
};