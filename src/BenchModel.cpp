#include "BenchModel.h"

BenchModel::BenchModel()
{
    vbo = new VBO();
    //Creating a VBO with raw data
    vbo->generateVBO(benchVertices, sizeof(benchVertices));
    
    vao = new VAO();
    vao->generateVAO();
    
    vao->bind(); // open vao
    vbo->bind(); // connect vbo to vao

    // VAO atribúty = vertex shader inputy
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);

    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                            (void*)(3 * sizeof(float)));
    
    vbo->unbind(); // disconnect vbo, vao is already saved
    vao->unbind();
}

void BenchModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 108); // pošli ich do vertex shaderu
    vao->unbind();
}