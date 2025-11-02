#include "VBO.h"

void VBO::generateVBO(const GLfloat* data, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID); // state machine
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID); // aktívny buffer s id pre operace GL_ARRAY_BUFFER
}

void VBO::unbind() // deactivate buffer
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}