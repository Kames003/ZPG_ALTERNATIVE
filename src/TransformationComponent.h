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
class TransformationComponent
{
protected:
    glm::mat4 matrix = glm::mat4(1.0f);
public:
    virtual glm::mat4 resultMatrix() = 0;
    virtual ~TransformationComponent() = default;
};