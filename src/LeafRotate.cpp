#include "LeafRotate.h"

LeafRotate::LeafRotate(float angle, glm::vec3 axis)
{
    // Konverzia uhla zo stupňov na radiány
    float r = glm::radians(angle);

    // Vytvorenie rotačnej matice pomocou GLM
    matrix = glm::rotate(glm::mat4(1.0f), r, axis);
}

glm::mat4 LeafRotate::resultMatrix()
{
    return matrix;
}