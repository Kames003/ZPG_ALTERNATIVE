#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

/**
 * @brief Abstraktná základná trieda pre transformácie
 *
 * Každá konkrétna transformácia (Rotate, Scale, Translate, Composite)
 * dedí z tejto triedy a implementuje metódu resultMatrix().
 *
 * Hierarchia:
 * Transformation (abstraktná)
 * ├── TransformationRotate
 * ├── TransformationScale
 * ├── TransformationTranslate
 * └── TransformationComposite
 */
class Transformation
{
protected:
    /**
     * @brief Výsledná transformačná matica
     *
     * Inicializovaná ako identita (žiadna transformácia)
     */
    glm::mat4 matrix = glm::mat4(1.0f);

public:
    /**
     * @brief Vráti transformačnú maticu
     *
     * Táto metóda musí byť implementovaná v každej konkrétnej triede.
     *
     * @return glm::mat4 Výsledná transformačná matica
     */
    virtual glm::mat4 resultMatrix() = 0;

    /**
     * @brief Virtuálny destruktor
     *
     * Umožňuje správne mazanie objektov cez pointer na Transformation
     */
    virtual ~Transformation() = default;
};