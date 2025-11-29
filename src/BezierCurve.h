#pragma once

#include <glm/mat4x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/vec3.hpp>

/**
 * @brief Kubická Bézierova krivka (stateless matematická trieda)
 *
 * Implementuje kubickú Bézierovu krivku pomocou maticového vzorca:
 *   P(t) = T * A * Bᵀ
 *   P'(t) = T' * A * Bᵀ  (tečný vektor)
 *
 * kde:
 *   T = [t³, t², t, 1]        - parameter vector
 *   T' = [3t², 2t, 1, 0]      - derivácia parametra (pre tangent)
 *   A = Bezier basis matrix   - charakteristická matica
 *   B = [P0, P1, P2, P3]ᵀ     - kontrolné body
 *
 * Trieda je PURE (bez vnútorného stavu) - výpočty závisia len od vstupného t.
 */
class BezierCurve
{
private:
	// Bézierova charakteristická matica (Cubic Bezier Basis Matrix)
	// Táto matica je ROVNAKÁ pre všetky kubické Bézierove krivky
	static const glm::mat4 A;

	glm::mat4x3 controlPoints;  // 4 kontrolné body [P0, P1, P2, P3]

public:
	/**
	 * Konštruktor - vytvorí krivku zo 4 kontrolných bodov
	 * @param points Matica 4x3 (4 body, každý má x,y,z súradnice)
	 */
	BezierCurve(const glm::mat4x3& points);

	/**
	 * Vypočíta bod na krivke pre parameter t ∈ [0, 1]
	 * Vzorec: P(t) = [t³, t², t, 1] * A * Bᵀ
	 *
	 * @param t Parameter krivky (0 = začiatok, 1 = koniec)
	 * @return 3D pozícia na krivke
	 */
	glm::vec3 getPoint(float t) const;

	/**
	 * Vypočíta tečný vektor (tangent) v bode t
	 * Vzorec: P'(t) = [3t², 2t, 1, 0] * A * Bᵀ
	 *
	 * Tečný vektor ukazuje SMER pohybu v danom bode.
	 * Je normalizovaný (jednotková dĺžka).
	 *
	 * @param t Parameter krivky (0 = začiatok, 1 = koniec)
	 * @return Normalizovaný tečný vektor
	 */
	glm::vec3 getTangent(float t) const;

	/**
	 * Vráti kontrolné body krivky
	 * @return Matica 4x3 s kontrolnými bodmi
	 */
	glm::mat4x3 getControlPoints() const { return controlPoints; }
};