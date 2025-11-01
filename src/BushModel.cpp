#include "BushModel.h"

BushModel::BushModel()
{
    vbo = new VBO();
    vbo->generateVBO(bushes, sizeof(bushes));  // Pozor: bushes, nie bush!
    
    vao = new VAO();
    vao->generateVAO();
    
    vao->bind();
    vbo->bind();
    
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                            (void*)(3 * sizeof(float)));
    
    vbo->unbind();
    vao->unbind();
}

void BushModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 8730);  // Krík má 8730 vrcholov
    vao->unbind();
}