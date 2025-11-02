#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileReader.h"

class Shader
{
protected:
    GLuint ID; // OpenGL id fan, every shader has id
    const char* shaderCode; // every shader has glsl code
public:
    virtual GLuint compileShader(const char* filename) = 0;
};

