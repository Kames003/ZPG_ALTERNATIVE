#pragma once

#include "TransformationComponent.h"

/**
 * @brief Všeobecná transformácia - akceptuje ľubovoľnú maticu
 *
 * Môžeš vytvoriť akúkoľvek custom maticu a pridať ju do composite.
 * Užitočné pre vlastné transformácie ako napr. zmena [3][3] pre perspektívne delenie.
 *
 * Príklad použitia:
 * @code
 * glm::mat4 customMatrix = glm::mat4(1.0f);
 * customMatrix[3][3] = 20.0f;  // Všetko bude 20x menšie
 *
 * drawable->customMatrix(customMatrix);
 * drawable->translate(glm::vec3(0.0f, 0.0f, 0.0f));
 * drawable->calculateModelMatrix();
 * @endcode
 */
class LeafMatrix : public TransformationComponent
{
public:
    LeafMatrix(const glm::mat4& m);
    glm::mat4 resultMatrix() override;
};
