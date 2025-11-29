#pragma once

#include "TransformationComponent.h"


class LeafCustomW : public TransformationComponent
{
public:
    LeafCustomW(float customW);
    glm::mat4 resultMatrix() override;
};