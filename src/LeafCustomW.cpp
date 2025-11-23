#include "LeafCustomW.h"

LeafCustomW::LeafCustomW(float customW)
{
    // Jednotkova matica s [3][3] = customW
    // GLM pouziva column-major order, takze matrix[col][row]
    // matrix[3][3] = element v 4. stlpci, 4. riadku
    matrix = glm::mat4(1.0f);
    matrix[3][3] = customW;
}

glm::mat4 LeafCustomW::resultMatrix()
{
    return matrix;
}