#pragma once

#include "AbstractScene.h"
#include "SphereModel.h"
#include "DrawableObject.h"
#include "TriangleModel.h"

/**
 * @brief Scéna 1: Phong Test s 3 svetlami
 * 
 * Testovacia scéna pre overenie:
 * - Phong osvietľovacieho modelu
 * - Viacerých svetiel naraz
 * - Správneho zmiešania farieb svetiel
 * 
 * Pozície svetiel:
 * - Svetlo 1 (biele):     [0.0,  0.0,  2.0]
 * - Svetlo 2 (červenkasté): [5.0,  3.0,  2.0]
 * - Svetlo 3 (modrasté):   [-5.0, 3.0,  2.0]
 */
class Scene1_PhongTest : public AbstractScene
{
private:
    float rotationAngle = 0.0f;

protected:
    void createShaders() override;
    void createDrawableObjects() override;
    void callbacks() override;

public:
    void createScene(GLFWwindow* window) override;
    void renderFrame() override;
    void renderScene() override;
};