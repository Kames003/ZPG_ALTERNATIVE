#pragma once

#include "TransformationComponent.h"
#include <vector>

/**
 *
 * !!!! na pripomenutie daj si na to pozor !!!!!!
 *
 * @brief Composite transformácií (Composite pattern)
 *

 * calculateTransformations() VYPRÁZDNI zoznam transformácií
 * Toto je užitočné pre animácie, kde chcete každý frame pridať
 * transformácie znova.
 *
 * Príklad použitia:
 * @code
 * TransformationComposite* tc = new TransformationComposite();
 *
 *
 * tc->addTransformation(new TransformationTranslate(glm::vec3(1, 0, 0)));
 * tc->addTransformation(new TransformationRotate(45.0f, glm::vec3(0, 1, 0)));
 * tc->addTransformation(new TransformationScale(glm::vec3(2, 2, 2)));
 *
 * // Vypočítaj výslednú maticu (zoznam sa vyprázdni!)
 * tc->calculateTransformations();
 *
 * // Získaj výslednú maticu
 * glm::mat4 modelMatrix = tc->getResultMatrix();
 * @endcode
 *
 * PORADIE TRANSFORMÁCIÍ:
 * Transformácie sa aplikujú v OPAČNOM poradí ako sa pridávajú!
 *
 * Príklad:
 * @code
 * tc->addTransformation(new TransformationTranslate(glm::vec3(5, 0, 0))); // 1.
 * tc->addTransformation(new TransformationRotate(90.0f, glm::vec3(0, 1, 0))); // 2.
 * tc->calculateTransformations();
 *
 * // Výsledok: Najprv ROTÁCIA, potom POSUNUTIE
 * // Objekt sa najprv otočí okolo origin, potom sa posunie vpravo
 * @endcode
 *
 *
 */
class TransformationComposite : public TransformationComponent
{
private:

	 // brief Zoznam transformácií
	std::vector<TransformationComponent*> componentCollection;

public:

	void addTransformation(TransformationComponent* t);
	void removeTransformations();
	void calculateTransformations();
	glm::mat4 resultMatrix();
	glm::mat4 getResultMatrix();
};