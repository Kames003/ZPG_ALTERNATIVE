#include "TriangleModel.h"

TriangleModel::TriangleModel()
{

    vbo = new VBO();
    vbo->generateVBO(triangle, sizeof(triangle));


    vao = new VAO();
    vao->generateVAO();


    vao->bind();
    vbo->bind();

   // position
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);

    // normala
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