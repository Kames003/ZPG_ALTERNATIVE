#include "SphereModel.h"

SphereModel::SphereModel()
{
    // 1. Vytvor VBO a naplň ho dátami
    vbo = new VBO();
    vbo->generateVBO(sphere, sizeof(sphere));
    
    // 2. Vytvor VAO - definuje ako čítať VBO
    vao = new VAO();
    vao->generateVAO();
    
    // 3. Nastavenie vertexových atribútov
    vao->bind();
    vbo->bind();
    
    // Atribút 0: pozícia (x, y, z) - 3 floaty
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    
    // Atribút 1: normála (nx, ny, nz) - 3 floaty, offset 3*sizeof(float)
    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                            (void*)(3 * sizeof(float)));
    
    vbo->unbind();
    vao->unbind();
}

void SphereModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 2880);  // 2880 vrcholov pre sféru
    vao->unbind();
}