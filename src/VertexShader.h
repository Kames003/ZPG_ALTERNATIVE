#pragma once
#include "Shader.h"
class VertexShader : public Shader
{
    GLuint compileShader(const char* filename);
};

