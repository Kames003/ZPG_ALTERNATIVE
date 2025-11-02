#pragma once

#include "TransformationComponent.h"


class LeafScale : public TransformationComponent
{
public:

    LeafScale(glm::vec3 scale);
    glm::mat4 resultMatrix();
};