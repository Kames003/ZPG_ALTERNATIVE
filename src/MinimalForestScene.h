// MinimalForestScene.h
#pragma once
#include "AbstractScene.h"

/**
 * @brief Minimálna scéna s lesom - len rovina a 20 stromov
 *
 * Účel: Jednoduchá testovacia scéna bez zbytočných komplikácií
 *
 * Obsahuje:
 * - 1 rovinú (zem) - index 0
 * - 20 stromov - náhodne rozmiestnených
 * - 1 PointLight
 * - Phong shading
 */
class MinimalForestScene : public AbstractScene
{
protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void createLights();
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderScene() override;
};