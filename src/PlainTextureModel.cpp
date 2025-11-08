#include "PlainTextureModel.h"

PlainTextureModel::PlainTextureModel()
{
    // Vytvor VBO s dátami z plain_texture
    vbo = new VBO();
    vbo->generateVBO(plain_texture, sizeof(plain_texture));
    
    // Vytvor VAO
    vao = new VAO();
    vao->generateVAO();
    
    // Nastavenie vertex atribútov
    vao->bind();
    vbo->bind();
    
    // Layout location 0: Pozícia (3 floaty)
    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    
    // Layout location 1: Normála (3 floaty)
    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
                            (void*)(3 * sizeof(float)));
    
    // Layout location 2: UV textúrové súradnice (2 floaty)
    vao->vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
                            (void*)(6 * sizeof(float)));
    
    vbo->unbind();
    vao->unbind();
}

void PlainTextureModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);  // 6 vrcholov (2 trojuholníky)
    vao->unbind();
}
