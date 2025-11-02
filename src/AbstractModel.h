#pragma once

#include "VBO.h"
#include "VAO.h"
/*
    @brief
*/

class AbstractModel
{
protected:
    VAO* vao;
    VBO* vbo;

public:
    virtual void draw() = 0;
    // virtual ~AbstractModel() = default;
};