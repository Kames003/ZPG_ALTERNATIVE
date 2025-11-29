#include "BezierCurve.h"
#include <glm/gtc/matrix_transform.hpp>

// Definícia statickej Bézierovej charakteristickej matice
// Táto matica je ROVNAKÁ pre všetky kubické Bézierove krivky
//
// Matica A transformuje kontrolné body na parametre polynómu:
//   [-1   3  -3   1]     [t³]
//   [ 3  -6   3   0]  *  [t²]  = koeficienty Bézierovej krivky
//   [-3   3   0   0]     [t ]
//   [ 1   0   0   0]     [1 ]
const glm::mat4 BezierCurve::A = glm::mat4(
	glm::vec4(-1.0f,  3.0f, -3.0f, 1.0f),   // prvý stĺpec
	glm::vec4( 3.0f, -6.0f,  3.0f, 0.0f),   // druhý stĺpec
	glm::vec4(-3.0f,  3.0f,  0.0f, 0.0f),   // tretí stĺpec
	glm::vec4( 1.0f,  0.0f,  0.0f, 0.0f)    // štvrtý stĺpec
);

BezierCurve::BezierCurve(const glm::mat4x3& points)
	: controlPoints(points)
{
	// Konštruktor len uloží body - žiadne výpočty
}

glm::vec3 BezierCurve::getPoint(float t) const
{
	// Zabezpeč, že t je v rozsahu [0, 1]
	t = glm::clamp(t, 0.0f, 1.0f);

	// Vypočítaj parameter vector T = [t³, t², t, 1]
	float t2 = t * t;       // t²
	float t3 = t2 * t;      // t³
	glm::vec4 T = glm::vec4(t3, t2, t, 1.0f);

	// Bézierova formula: P(t) = T * A * Bᵀ
	// kde B je matica kontrolných bodov (4x3)
	glm::vec3 point = T * A * glm::transpose(controlPoints);

	return point;
}

glm::vec3 BezierCurve::getTangent(float t) const
{
	// Zabezpeč, že t je v rozsahu [0, 1]
	t = glm::clamp(t, 0.0f, 1.0f);

	// Vypočítaj DERIVÁCIU parameter vectora T' = [3t², 2t, 1, 0]
	// (derivácia t³ = 3t², derivácia t² = 2t, derivácia t = 1, derivácia 1 = 0)
	float t2 = t * t;       // t²
	glm::vec4 T_prime = glm::vec4(3.0f * t2, 2.0f * t, 1.0f, 0.0f);

	// Tečný vektor: P'(t) = T' * A * Bᵀ
	glm::vec3 tangent = T_prime * A * glm::transpose(controlPoints);

	// Normalizuj (jednotková dĺžka)
	float length = glm::length(tangent);
	if (length > 0.0001f) {
		return tangent / length;
	}

	// Fallback - ak je tangent nulový (degenerate prípad)
	return glm::vec3(0.0f, 0.0f, 1.0f);
}