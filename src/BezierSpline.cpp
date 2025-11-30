#include "BezierSpline.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

BezierSpline::BezierSpline()
	: globalParameter(0.0f)
	, delta(0.005f)
	, loop(false)
	, pingPong(true)
	, forward(true)
{
}

BezierSpline::~BezierSpline()
{
	for (auto segment : segments) {
		delete segment;
	}
}

void BezierSpline::addControlPoint(glm::vec3 point)
{
	controlPoints.push_back(point);

	// Prebuduj segmenty ak máme dosť bodov
	if (controlPoints.size() >= 4) {
		rebuildSegments();
	}
}

void BezierSpline::setControlPoints(const std::vector<glm::vec3>& points)
{
	clearControlPoints();
	controlPoints = points;

	if (controlPoints.size() >= 4) {
		rebuildSegments();
	}
}

void BezierSpline::clearControlPoints()
{
	controlPoints.clear();
	for (auto segment : segments) {
		delete segment;
	}
	segments.clear();
	globalParameter = 0.0f;
}

void BezierSpline::rebuildSegments()
{
	for (auto segment : segments) {
		delete segment;
	}
	segments.clear();

	if (controlPoints.size() < 4) {
		return;
	}

	size_t numSegments = (controlPoints.size() - 1) / 3;

	for (size_t i = 0; i < numSegments; i++)
	{
		size_t baseIndex = i * 3;

		glm::mat4x3 segmentPoints = glm::mat4x3(
			controlPoints[baseIndex],
			controlPoints[baseIndex + 1],
			controlPoints[baseIndex + 2],
			controlPoints[baseIndex + 3]
		);

		BezierCurve* segment = new BezierCurve(segmentPoints);
		segments.push_back(segment);
	}

	printf("BezierSpline: Created %zu segments from %zu control points\n",
		segments.size(), controlPoints.size());
}

int BezierSpline::getCurrentSegmentIndex(float globalT) const
{
	if (segments.empty()) return -1;

	int index = static_cast<int>(globalT * segments.size());
	if (index >= static_cast<int>(segments.size())) {
		index = segments.size() - 1;
	}
	return index;
}

float BezierSpline::getLocalParameter(float globalT, int segmentIndex) const
{
	if (segments.empty()) return 0.0f;

	float segmentSize = 1.0f / segments.size();
	float segmentStart = segmentIndex * segmentSize;
	float localT = (globalT - segmentStart) / segmentSize;

	return glm::clamp(localT, 0.0f, 1.0f);
}

glm::vec3 BezierSpline::calculatePoint(float globalT)
{
	if (segments.empty()) {
		return glm::vec3(0.0f);
	}

	int segmentIndex = getCurrentSegmentIndex(globalT);
	float localT = getLocalParameter(globalT, segmentIndex);

	return segments[segmentIndex]->getPoint(localT);
}

glm::vec3 BezierSpline::calculatePoint()
{
	return calculatePoint(globalParameter);
}

glm::vec3 BezierSpline::calculateTangent(float globalT)
{
	if (segments.empty()) {
		return glm::vec3(0.0f, 0.0f, 1.0f);
	}

	int segmentIndex = getCurrentSegmentIndex(globalT);
	float localT = getLocalParameter(globalT, segmentIndex);

	return segments[segmentIndex]->getTangent(localT);
}

glm::vec3 BezierSpline::calculateTangent()
{
	return calculateTangent(globalParameter);
}

void BezierSpline::update(float deltaTime)
{
	if (segments.empty()) {
		return;
	}

	// ✅ OPRAVA: Vypočítaj BUDÚCI parameter
	float nextParameter = globalParameter;
	if (forward) {
		nextParameter += delta;
	} else {
		nextParameter -= delta;
	}

	// ✅ OPRAVA: Kontroluj hranice PRED aplikovaním zmeny
	if (nextParameter >= 1.0f)
	{
		if (pingPong) {
			// ✅ SMOOTH BOUNCE: Použij nadmernú časť (overshoot) v opačnom smere
			float overshoot = nextParameter - 1.0f;  // Koľko sme prekročili?
			globalParameter = 1.0f - overshoot;      // Odrátaj od konca
			forward = false;
			printf("[BEZIER] Reached end (t=1.0), reversing direction (overshoot=%.4f)\n", overshoot);
		} else if (loop) {
			globalParameter = 0.0f;
		} else {
			globalParameter = 1.0f;
		}
	}
	else if (nextParameter <= 0.0f)
	{
		if (pingPong) {
			// ✅ SMOOTH BOUNCE: Použij nadmernú časť (undershoot) v opačnom smere
			float undershoot = -nextParameter;       // Koľko sme prekročili pod 0?
			globalParameter = 0.0f + undershoot;     // Pripočítaj od začiatku
			forward = true;
			printf("[BEZIER] Reached start (t=0.0), reversing direction (undershoot=%.4f)\n", undershoot);
		} else if (loop) {
			globalParameter = 1.0f;
		} else {
			globalParameter = 0.0f;
		}
	}
	else
	{
		// Normálny update - parameter je v rozsahu [0, 1]
		globalParameter = nextParameter;
	}
}