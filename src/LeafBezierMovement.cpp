#include "LeafBezierMovement.h"
#include <glm/gtc/matrix_transform.hpp>

LeafBezierMovement::LeafBezierMovement(BezierSpline* bezierSpline)
	: spline(bezierSpline)
	, rotateAlongPath(true)
	, upVector(0.0f, 1.0f, 0.0f)
{
	matrix = glm::mat4(1.0f);
	

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

	glm::vec3 forward = glm::normalize(tangent);

	glm::vec3 right = glm::cross(upVector, forward);
	float rightLength = glm::length(right);
	
	if (rightLength < 0.001f) {

		right = glm::vec3(1.0f, 0.0f, 0.0f);  // X os ako základný right vektor
	} else {
		right = right / rightLength;  // Normalizuj manuálne
	}

	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0] = glm::vec4(right, 0.0f);
	rotation[1] = glm::vec4(up, 0.0f);
	rotation[2] = glm::vec4(forward, 0.0f);

	return rotation;
}

void LeafBezierMovement::update(float deltaTime)
{
	if (!spline || !spline->isValid()) {
		matrix = glm::mat4(1.0f);
		return;
	}


	spline->update(deltaTime);
	glm::vec3 position = spline->calculatePoint();
	glm::vec3 tangent = spline->calculateTangent();

	if (!spline->isGoingForward()) {
		tangent = -tangent;  // Invertuj smer!
	}

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
