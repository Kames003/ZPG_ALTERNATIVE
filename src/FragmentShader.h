#pragma once
#include "Shader.h"

class FragmentShader : public Shader
{
    GLuint compileShader(const char* filename);
};

