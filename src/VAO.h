#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VAO
{
private:
    GLuint ID;
public:
    void generateVAO();
    void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    void bind();
    void unbind();
};

