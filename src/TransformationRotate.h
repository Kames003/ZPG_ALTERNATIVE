#pragma once

#include "Transformation.h"

/**
 * @brief Transformácia rotácie
 *
 * Vytvára rotačnú maticu okolo zadanej osi o zadaný uhol.
 *
 * Príklad použitia:
 * @code
 * // Rotácia o 45° okolo Y osi
 * Transformation* rot = new TransformationRotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
 * glm::mat4 matrix = rot->resultMatrix();
 * @endcode
 */
class TransformationRotate : public Transformation
{
public:
    /**
     * @brief Konštruktor
     *
     * @param angle Uhol rotácie v stupňoch (nie radiánoch!)
     * @param axis Os rotácie (napr. glm::vec3(0, 1, 0) pre Y os)
     *
     * Bežné osi:
     * - X os: glm::vec3(1, 0, 0)
     * - Y os: glm::vec3(0, 1, 0)
     * - Z os: glm::vec3(0, 0, 1)
     */
    TransformationRotate(float angle, glm::vec3 axis);


    glm::mat4 resultMatrix();
};