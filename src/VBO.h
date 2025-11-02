#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
@brief
* wrapper class
@endcode
*/

class VBO
{
private:
    GLuint ID;
public:
    void generateVBO(const GLfloat* data, GLsizeiptr size);
    void bind();
    void unbind();
};

