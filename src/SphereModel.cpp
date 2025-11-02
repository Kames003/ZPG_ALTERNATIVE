#include "SphereModel.h"

SphereModel::SphereModel()
{

    // - Na GPU sa vytvoril buffer s ID (napr. 42)
    // - Do neho sa skopírovalo 69,120 bytov (2880 vrcholov × 24 bytov)

    vbo = new VBO();
    vbo->generateVBO(sphere, sizeof(sphere));

    // - Na GPU sa vytvoril prázdny VAO s ID (napr. 7)
    // - Je to zatiaľ prázdna škatuľa, musíme ju naplniť návodo

    vao = new VAO();
    vao->generateVAO();
    

    vao->bind(); // otvor / aktivuj vao s číslom 7
    vbo->bind(); // vao s číslom 7 bude čítať z vbo 42

    // vao 7 je spojené s vbo 42
    

    vao->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    

    vao->vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                            (void*)(3 * sizeof(float)));
    
    vbo->unbind();
    vao->unbind(); // zatvor vao navod je  uložený
}

void SphereModel::draw()
{
    vao->bind(); // použi návod zo z vao 7
    glDrawArrays(GL_TRIANGLES, 0, 2880);  // nakresli 2880 vrcholov
    vao->unbind();
}