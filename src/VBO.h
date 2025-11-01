#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO
{
private:
    GLuint ID;
public:
    void generateVBO(const GLfloat* data, GLsizeiptr size);
    void bind();
    void unbind();
};

