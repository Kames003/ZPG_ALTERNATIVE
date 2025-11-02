#include "TreeModel.h"

TreeModel::TreeModel()
{
    vbo = new VBO();
    vbo->generateVBO(tree, sizeof(tree));
    
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

void TreeModel::draw()
{
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 92814);
    vao->unbind();
}