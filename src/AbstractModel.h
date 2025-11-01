#pragma once

#include "VBO.h"
#include "VAO.h"

// Abstraktná základná trieda pre všetky 3D modely
// Poskytuje spoločné rozhranie pre prácu s VBO a VAO
class AbstractModel
{
protected:
    VAO* vao;  // Vertex Array Object - definuje ako čítať data z VBO
    VBO* vbo;  // Vertex Buffer Object - obsahuje vertex data

public:
    // Čisto virtuálna metóda - každý konkrétny model ju musí implementovať
    virtual void draw() = 0;
    
    // virtual ~AbstractModel() = default;  // Virtuálny destruktor pre správne mazanie
};