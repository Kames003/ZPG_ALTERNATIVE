#include "BezierCurve.h"
#include <glm/gtc/matrix_transform.hpp>

// Matica A transformuje kontrolné body na parametre polynómu:
//   [-1   3  -3   1]     [t³]
//   [ 3  -6   3   0]  *  [t²]
//   [-3   3   0   0]     [t ]
//   [ 1   0   0   0]     [1 ]
const glm::mat4 BezierCurve::A = glm::mat4(
	glm::vec4(-1.0f,  3.0f, -3.0f, 1.0f),
	glm::vec4( 3.0f, -6.0f,  3.0f, 0.0f),
	glm::vec4(-3.0f,  3.0f,  0.0f, 0.0f),
	glm::vec4( 1.0f,  0.0f,  0.0f, 0.0f)
);

BezierCurve::BezierCurve(const glm::mat4x3& points)
	: controlPoints(points)
{
	// Konštruktor len uloží body - žiadne výpočty
}

glm::vec3 BezierCurve::getPoint(float t) const
{
	t = glm::clamp(t, 0.0f, 1.0f);
	float t2 = t * t;
	float t3 = t2 * t;
	glm::vec4 T = glm::vec4(t3, t2, t, 1.0f);
	glm::vec3 point = T * A * glm::transpose(controlPoints);

	return point;
}

glm::vec3 BezierCurve::getTangent(float t) const
{

	t = glm::clamp(t, 0.0f, 1.0f);
	float t2 = t * t;       // t²
	glm::vec4 T_prime = glm::vec4(3.0f * t2, 2.0f * t, 1.0f, 0.0f);

	glm::vec3 tangent = T_prime * A * glm::transpose(controlPoints);

	float length = glm::length(tangent);
	if (length > 0.0001f) {
		return tangent / length;
	}

	return glm::vec3(0.0f, 0.0f, 1.0f);
}