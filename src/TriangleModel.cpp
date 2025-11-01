#include "TriangleModel.h"

TriangleModel::TriangleModel()
{
    // Vytvor VBO s dátami trojuholníka
    vbo = new VBO();
    vbo->generateVBO(triangle, sizeof(triangle));

    // Vytvor VAO
    vao = new VAO();
    vao->generateVAO();

    // Nastavenie vertex atribútov
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

void TriangleModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);  // 3 vrcholy = 1 trojuholník
    vao->unbind();
}