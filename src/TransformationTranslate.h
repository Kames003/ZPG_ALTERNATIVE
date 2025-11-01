#pragma once

#include "Transformation.h"

/**
 * @brief Transformácia posunutia
 *
 * Vytvára maticu pre posunutie objektu v priestore.
 *
 * Príklad použitia:
 * @code
 * // Posunutie o 5 metrov doprava
 * Transformation* trans = new TransformationTranslate(glm::vec3(5.0f, 0.0f, 0.0f));
 *
 * // Posunutie hore
 * Transformation* trans2 = new TransformationTranslate(glm::vec3(0.0f, 2.0f, 0.0f));
 *
 * // Posunutie v 3D priestore
 * Transformation* trans3 = new TransformationTranslate(glm::vec3(1.0f, 2.0f, 3.0f));
 * @endcode
 */
class TransformationTranslate : public Transformation
{
public:
    /**
     * @brief Konštruktor
     *
     * @param translate Vektor posunutia (x, y, z)
     *
     * Osi (pravotočivý súradnicový systém):
     * - X os: vpravo/vľavo
     * - Y os: hore/dole
     * - Z os: dopredu/dozadu (záporné Z = do scény)
     *
     * Príklady:
     * - glm::vec3(5, 0, 0) = 5 metrov doprava
     * - glm::vec3(0, -2, 0) = 2 metre dole
     * - glm::vec3(0, 0, -10) = 10 metrov do scény
     */
    TransformationTranslate(glm::vec3 translate);

    /**
     * @brief Vráti maticu posunutia
     *
     * @return glm::mat4 Matica posunutia
     */
    glm::mat4 resultMatrix();
};