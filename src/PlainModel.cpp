#include "PlainModel.h"

PlainModel::PlainModel()
{
    vbo = new VBO();
    vbo->generateVBO(plain, sizeof(plain));
    
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

void PlainModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Rovina má len 6 vrcholov (2 trojuholníky)
    vao->unbind();
}