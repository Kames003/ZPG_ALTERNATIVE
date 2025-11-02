#include "TransformationComposite.h"

void TransformationComposite::addTransformation(TransformationComponent* t)
{
    // Pridáme transformáciu do zoznamu
    transformations.push_back(t);
}

void TransformationComposite::removeTransformations()
{
    // Vyprázdnime zoznam transformácií
    transformations.clear();
}

void TransformationComposite::calculateTransformations()
{
    // Začneme s identity maticou (žiadna transformácia)
    this->matrix = glm::mat4(1.0f);

    // Vynásobíme postupne všetkými transformáciami
    for (auto t : transformations)
    {
        matrix = matrix * t->resultMatrix();
    }

    // Vyprázdnime zoznam transformácií
    //  animácie - každý frame pridáme nové transformácie
    removeTransformations();
}

glm::mat4 TransformationComposite::resultMatrix()
{
    // Začneme s identity maticou
    this->matrix = glm::mat4(1.0f);

    // Vynásobíme postupne všetkými transformáciami
    for (auto t : transformations)
    {
        this->matrix = this->matrix * t->resultMatrix();
    }

    // Vrátíme výsledok BEZ vyprázdnenia zoznamu
    return matrix;
}

glm::mat4 TransformationComposite::getResultMatrix()
{
    // Vrátíme uloženú maticu (bez prepočtu)
    return this->matrix;
}