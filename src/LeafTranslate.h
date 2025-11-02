#pragma once

#include "TransformationComponent.h"


class LeafTranslate : public TransformationComponent
{
public:

    LeafTranslate(glm::vec3 translate);
    glm::mat4 resultMatrix();
};