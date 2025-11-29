#pragma once

#include "PointLight.h"
#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>


class Firefly
{
private:
    PointLight* light;
    DrawableObject* visual;

    // Animace
    glm::vec3 centerPosition;
    float orbitRadius;
    float orbitSpeed;
    float verticalSpeed;
    float phase;

    // Vlastnosti
    glm::vec3 color;
    float intensity;
    float minIntensity;
    float maxIntensity;
    float blinkSpeed;


    glm::vec3 visualScale;

public:

    Firefly(glm::vec3 position, 
            AbstractModel* model, 
            ShaderProgram* shader,
            glm::vec3 lightColor = glm::vec3(1.0f, 0.9f, 0.5f));
    

    void updateAnimation(float time);
    void setCenterPosition(glm::vec3 pos);
    void setOrbitRadius(float radius);
    void setSpeed(float speed);
    PointLight* getLight();
    DrawableObject* getVisual();
    void setVisualVisible(bool visible);
};