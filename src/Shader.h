#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileReader.h"

class Shader
{
protected:
    GLuint ID;
    const char* shaderCode;
public:
    virtual GLuint compileShader(const char* filename) = 0;
};

