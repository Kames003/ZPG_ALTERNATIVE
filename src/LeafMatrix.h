#pragma once

#include "TransformationComponent.h"

class LeafMatrix : public TransformationComponent
{
public:
    LeafMatrix(const glm::mat4& m);
    glm::mat4 resultMatrix() override;
};
