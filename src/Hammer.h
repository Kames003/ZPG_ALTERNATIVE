#pragma once

#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>


class Hammer
{
private:
    DrawableObject* visual;


    glm::vec3 position;


    bool visible;
    float visibleTimer;
    float visibleDuration;


    float scale;

public:
    Hammer(AbstractModel* model, ShaderProgram* shader);
    ~Hammer();


    void setScale(float s) { scale = s; }
    void setVisibleDuration(float d) { visibleDuration = d; }

    void update(float deltaTime);

    void strike(glm::vec3 pos);

    bool isVisible() const { return visible; }
    DrawableObject* getVisual() { return visual; }
};
