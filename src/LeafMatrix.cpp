#include "LeafMatrix.h"

LeafMatrix::LeafMatrix(const glm::mat4& m)
{
    // Ulož ľubovoľnú maticu
    matrix = m;
}

glm::mat4 LeafMatrix::resultMatrix()
{
    return matrix;
}