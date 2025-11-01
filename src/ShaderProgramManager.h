#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <stdio.h>
#include "ShaderProgram.h"
#include "LightManager.h"

// Manager pre správu všetkých shader programov
// Vie updatovať všetky shadery naraz (svetlá, kameru, atď.)
class ShaderProgramManager
{
private:
    std::vector<ShaderProgram*> shaderPrograms;
    LightManager* lm;  // Referencia na light manager

    // Private metódy pre update svetiel
    void updateAmbient();
    void updatePointLights();
    void updateSpotlights();
    void updateSpotlightCamera();
    void updateDirectionalLight();

public:
    ShaderProgramManager(LightManager* lm);

    // Pridanie shader programu
    void addShaderProgram(Camera* c, const char* vertexShaderFile,
                          const char* fragmentShaderFile);

    // Získanie shader programu
    ShaderProgram* getShaderProgram(int index);

    // Update všetkých typov svetiel do všetkých shaderov
    void updateLights();
};