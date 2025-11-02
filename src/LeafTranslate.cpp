#include "LeafTranslate.h"

LeafTranslate::LeafTranslate(glm::vec3 translate)
{
    // Vytvorenie matice sclae pomocou GLM
    matrix = glm::translate(glm::mat4(1.0f), translate);
}

glm::mat4 LeafTranslate::resultMatrix()
{
    return matrix;
}