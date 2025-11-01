#pragma once

#include "Transformation.h"
#include <vector>

/**
 * @brief Composite transformácií (Composite pattern)
 *
 * Uchováva zoznam transformácií a kombinuje ich do jednej výslednej matice.
 * Implementuje Composite pattern z návrhových vzorov.
 *
 * Použitie:
 * 1. Pridaj transformácie: addTransformation(new TransformationTranslate(...))
 * 2. Vypočítaj výslednú maticu: calculateTransformations()
 * 3. Získaj výslednú maticu: getResultMatrix()
 *
 * DÔLEŽITÉ: calculateTransformations() VYPRÁZDNI zoznam transformácií!
 * Toto je užitočné pre animácie, kde chcete každý frame pridať
 * transformácie znova.
 *
 * Príklad použitia:
 * @code
 * TransformationComposite* tc = new TransformationComposite();
 *
 * // Pridaj transformácie
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
 * Ak chcete najprv posunúť a potom otočiť, pridajte ich v opačnom poradí.
 */
class TransformationComposite : public Transformation
{
private:
	/**
	 * @brief Zoznam transformácií
	 *
	 * Uchováva pointre na všetky transformácie v composite
	 */
	std::vector<Transformation*> transformations;

public:
	/**
	 * @brief Pridá transformáciu do zoznamu
	 *
	 * @param t Pointer na transformáciu (Rotate, Scale, Translate)
	 *
	 * Príklad:
	 * @code
	 * tc->addTransformation(new TransformationTranslate(glm::vec3(1, 0, 0)));
	 * @endcode
	 */
	void addTransformation(Transformation* t);

	/**
	 * @brief Odstráni všetky transformácie zo zoznamu
	 *
	 * Vyprázdni vector transformations.
	 * Voláné automaticky v calculateTransformations().
	 */
	void removeTransformations();

	/**
	 * @brief Vypočíta výslednú maticu z všetkých transformácií
	 *
	 * DÔLEŽITÉ: Táto metóda:
	 * 1. Vytvorí identity maticu (jednotková matica)
	 * 2. Vynásobí ju postupne všetkými transformáciami
	 * 3. Uloží výsledok do this->matrix
	 * 4. VYPRÁZDNI zoznam transformácií (volá removeTransformations())!
	 *
	 * Prečo sa zoznam vyprázdni?
	 * - Pre animácie: Každý frame pridáte nové transformácie a prepočítate
	 * - Predchádza sa akumulácii transformácií
	 *
	 * Použitie:
	 * @code
	 * // Frame 1:
	 * tc->addTransformation(new TransformationTranslate(glm::vec3(x1, y1, z1)));
	 * tc->calculateTransformations(); // Vyprázdni zoznam
	 *
	 * // Frame 2:
	 * tc->addTransformation(new TransformationTranslate(glm::vec3(x2, y2, z2)));
	 * tc->calculateTransformations(); // Vyprázdni zoznam znova
	 * @endcode
	 */
	void calculateTransformations();

	/**
	 * @brief Vráti výslednú maticu BEZ vyprázdnenia zoznamu
	 *
	 * Táto metóda:
	 * 1. Vytvorí identity maticu
	 * 2. Vynásobí ju postupne všetkými transformáciami
	 * 3. Vráti výsledok
	 * 4. NEVYPRÁZDNI zoznam (na rozdiel od calculateTransformations())
	 *
	 * Použitie: Keď chcete získať maticu bez zmeny stavu composite.
	 *
	 * @return glm::mat4 Výsledná transformačná matica
	 */
	glm::mat4 resultMatrix();

	/**
	 * @brief Získa uloženú výslednú maticu
	 *
	 * Vráti maticu ktorá bola vypočítaná pomocou calculateTransformations().
	 * Nerobí žiadne prepočty, iba vráti už existujúcu hodnotu this->matrix.
	 *
	 * Použitie:
	 * @code
	 * tc->calculateTransformations(); // Vypočítaj
	 * glm::mat4 m = tc->getResultMatrix(); // Získaj výsledok
	 * @endcode
	 *
	 * @return glm::mat4 Uložená výsledná matica
	 */
	glm::mat4 getResultMatrix();
};