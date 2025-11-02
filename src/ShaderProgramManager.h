#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <stdio.h>
#include "ShaderProgram.h"
#include "LightManager.h"

/*

@brief

Na čo slúži ?

updatesvetla
add shader
get shader

*/
class ShaderProgramManager
{
private:
    std::vector<ShaderProgram*> shaderPrograms;
    LightManager* lm;  // Referencia na light manager

    // light update
    void updateAmbient();
    void updatePointLights();
    void updateSpotlights();
    void updateSpotlightCamera();
    void updateDirectionalLight();

public:
    ShaderProgramManager(LightManager* lm);

    // add
    void addShaderProgram(Camera* c, const char* vertexShaderFile,
                          const char* fragmentShaderFile);

    // get
    ShaderProgram* getShaderProgram(int index);

    // upd lights
    void updateLights();
};