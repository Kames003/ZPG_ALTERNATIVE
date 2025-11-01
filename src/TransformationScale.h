#pragma once

#include "Transformation.h"

/**
 * @brief Transformácia škálovania
 *
 * Vytvára škálovaciu maticu pre zmenu veľkosti objektu.
 *
 * Príklad použitia:
 * @code
 * // Zväčšenie na dvojnásobok
 * Transformation* scale = new TransformationScale(glm::vec3(2.0f, 2.0f, 2.0f));
 *
 * // Zmenšenie na polovicu
 * Transformation* scale2 = new TransformationScale(glm::vec3(0.5f, 0.5f, 0.5f));
 *
 * // Neštvorcové škálovanie (zploštenie)
 * Transformation* scale3 = new TransformationScale(glm::vec3(1.0f, 0.5f, 1.0f));
 * @endcode
 */
class TransformationScale : public Transformation
{
public:
    /**
     * @brief Konštruktor
     *
     * @param scale Škálovací faktor pre každú os
     *
     * Hodnoty:
     * - 1.0 = žiadna zmena
     * - > 1.0 = zväčšenie
     * - < 1.0 = zmenšenie
     * - < 0.0 = zrkadlenie (nie je odporúčané)
     *
     * Príklady:
     * - glm::vec3(2.0f) = zväčšenie na dvojnásobok vo všetkých osiach
     * - glm::vec3(0.5f, 1.0f, 0.5f) = zmenšenie v X a Z, bez zmeny v Y
     */
    TransformationScale(glm::vec3 scale);

    /**
     * @brief Vráti škálovaciu maticu
     *
     * @return glm::mat4 Škálovacia matica
     */
    glm::mat4 resultMatrix();
};