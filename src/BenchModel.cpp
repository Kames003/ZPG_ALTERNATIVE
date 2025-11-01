#include "BenchModel.h"

BenchModel::BenchModel()
{
    vbo = new VBO();
    vbo->generateVBO(benchVertices, sizeof(benchVertices));  // Pozor: benchVertices!
    
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

void BenchModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 108);  // Lavička má 108 vrcholov
    vao->unbind();
}