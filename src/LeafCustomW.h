#pragma once

#include "TransformationComponent.h"

/**
 * @brief Vlastna transformacia s upravenou homogennou zlozkou w
 *
 * Vytvori jednotkovu maticu kde [3][3] = customW namiesto 1.0
 * Pouzitie: testovanie homogennych suradnic
 *
 * Priklad s w = 20:
 * | 1  0  0  0 |
 * | 0  1  0  0 |
 * | 0  0  1  0 |
 * | 0  0  0  20|
 *
 * Vysledok: objekt bude w-krat mensi (perspektivne delenie)
 */
class LeafCustomW : public TransformationComponent
{
public:
    LeafCustomW(float customW);
    glm::mat4 resultMatrix() override;
};