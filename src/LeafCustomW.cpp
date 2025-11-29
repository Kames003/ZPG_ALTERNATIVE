#include "LeafCustomW.h"

LeafCustomW::LeafCustomW(float customW)
{
    // Jednotkova matica s [3][3] = customW
    matrix = glm::mat4(1.0f);
    matrix[3][3] = customW;
}

glm::mat4 LeafCustomW::resultMatrix()
{
    return matrix;
}