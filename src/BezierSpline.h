#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "BezierCurve.h"

/**
 * @brief Kubická Bézierova spline křivka
 *
 * Skladá sa z viacerých navazujúcich kubických Bézierových segmentov.
 * Každý segment má 4 kontrolné body, pričom koniec jedného segmentu
 * je začiatkom nasledujúceho.
 *
 * Príklad: 7 bodov -> 2 segmenty:
 *   Segment 0: P0, P1, P2, P3
 *   Segment 1: P3, P4, P5, P6
 */
class BezierSpline
{
private:
	std::vector<glm::vec3> controlPoints;      // Všetky kontrolné body
	std::vector<BezierCurve*> segments;        // Segmenty (každý = 4 body)

	float globalParameter;  // Parameter pre celú spline (0-1)
	float delta;           // Krok animácie
	bool loop;             // Opakuj spline dookola
	bool pingPong;         // Tam a späť
	bool forward;          // Smer pohybu

	void rebuildSegments();  // Prebuduj segmenty z kontrolných bodov
	int getCurrentSegmentIndex(float globalT) const;
	float getLocalParameter(float globalT, int segmentIndex) const;

public:
	BezierSpline();
	~BezierSpline();

	// Pridávanie bodov
	void addControlPoint(glm::vec3 point);
	void setControlPoints(const std::vector<glm::vec3>& points);
	void clearControlPoints();

	// Výpočty
	glm::vec3 calculatePoint();              // Aktuálny bod
	glm::vec3 calculatePoint(float globalT); // Bod v danom t
	glm::vec3 calculateTangent();            // Aktuálny tangent
	glm::vec3 calculateTangent(float globalT); // Tangent v danom t

	// Animácia
	void update(float deltaTime);  // Automatický pohyb
	void setSpeed(float speed) { delta = speed; }
	void setLoop(bool l) { loop = l; }
	void setPingPong(bool pp) { pingPong = pp; }

	// Getters
	size_t getControlPointCount() const { return controlPoints.size(); }
	size_t getSegmentCount() const { return segments.size(); }
	bool isValid() const { return segments.size() > 0; }
	float getGlobalParameter() const { return globalParameter; }
	bool isGoingForward() const { return forward; }  // Či ideme forward alebo backward
	const std::vector<glm::vec3>& getControlPoints() const { return controlPoints; }
};
