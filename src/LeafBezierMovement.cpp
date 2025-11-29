#include "LeafBezierMovement.h"
#include <glm/gtc/matrix_transform.hpp>

LeafBezierMovement::LeafBezierMovement(BezierSpline* bezierSpline)
	: spline(bezierSpline)
	, rotateAlongPath(true)
	, upVector(0.0f, 1.0f, 0.0f)
{
	matrix = glm::mat4(1.0f);
	
	// ✅ Inicializuj maticu hneď, ak je spline validná
	if (spline && spline->isValid()) {
		glm::vec3 position = spline->calculatePoint();
		glm::vec3 tangent = spline->calculateTangent();
		matrix = glm::translate(glm::mat4(1.0f), position);
		if (rotateAlongPath) {
			matrix = matrix * calculateRotationMatrix(tangent);
		}
	}
}

LeafBezierMovement::~LeafBezierMovement()
{
	// Spline sa maže externe (scene management)
}

glm::mat4 LeafBezierMovement::calculateRotationMatrix(const glm::vec3& tangent)
{
	// Vytvoríme rotačnú maticu PRIAMO z tangent vectora
	// Použijeme cross products pre ortogonálnu bázu

	glm::vec3 forward = glm::normalize(tangent);

	// ✅ OPRAVA GIMBAL LOCK: Skontroluj, či forward nie je paralelný s upVector
	glm::vec3 right = glm::cross(upVector, forward);
	float rightLength = glm::length(right);
	
	if (rightLength < 0.001f) {
		// Forward je paralelný s upVector - použi fallback
		right = glm::vec3(1.0f, 0.0f, 0.0f);  // X os ako základný right vektor
	} else {
		right = right / rightLength;  // Normalizuj manuálne
	}

	// Vypočítaj správny up vektor (kolmý na forward a right)
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	// Vytvor rotačnú maticu (column-major!)
	// Shrek model pozerá v +Z smere (nie -Z ako OpenGL default)
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0] = glm::vec4(right, 0.0f);       // X osa = right
	rotation[1] = glm::vec4(up, 0.0f);          // Y osa = up
	rotation[2] = glm::vec4(forward, 0.0f);     // Z osa = forward (BEZ negácie)

	return rotation;
}

void LeafBezierMovement::update(float deltaTime)
{
	if (!spline || !spline->isValid()) {
		matrix = glm::mat4(1.0f);
		return;
	}

	// Update spline parameter
	spline->update(deltaTime);

	// Ziskaj poziciu a tangent
	glm::vec3 position = spline->calculatePoint();
	glm::vec3 tangent = spline->calculateTangent();

	// ✅ PING-PONG FIX: Pri backward pohybe invertuj tangent!
	// Tangent ukazuje "prirodzený smer krivky" (t=0 → t=1)
	// Pri backward (t=1 → t=0) musíme tangent otočiť o 180°
	if (!spline->isGoingForward()) {
		tangent = -tangent;  // Invertuj smer!
	}

	// Vytvor transformačnú maticu
	matrix = glm::translate(glm::mat4(1.0f), position);

	if (rotateAlongPath) {
		glm::mat4 rotation = calculateRotationMatrix(tangent);
		matrix = matrix * rotation;
	}
}

glm::mat4 LeafBezierMovement::resultMatrix()
{
	return matrix;
}
