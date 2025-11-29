#pragma once

#include "TransformationComponent.h"
#include "BezierSpline.h"

/**
 * @brief Transformačný komponent pre pohyb po Bézierovej spline
 *
 * Integruje BezierSpline do transformation systému.
 * Kombinuje translate + rotate podľa tečného vektora.
 */
class LeafBezierMovement : public TransformationComponent
{
private:
	BezierSpline* spline;
	bool rotateAlongPath;  // Rotovať podľa tečného vektora?
	glm::vec3 upVector;    // Up vector pre rotáciu

	glm::mat4 calculateRotationMatrix(const glm::vec3& tangent);

public:
	LeafBezierMovement(BezierSpline* bezierSpline);
	~LeafBezierMovement();

	void setRotateAlongPath(bool rotate) { rotateAlongPath = rotate; }
	void setUpVector(const glm::vec3& up) { upVector = up; }

	void update(float deltaTime);  // Volať každý frame

	// TransformationComponent interface
	glm::mat4 resultMatrix() override;

	BezierSpline* getSpline() { return spline; }
};
