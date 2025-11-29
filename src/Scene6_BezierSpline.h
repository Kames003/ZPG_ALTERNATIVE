#pragma once

#include "AbstractScene.h"
#include "BezierSpline.h"
#include "LeafBezierMovement.h"
#include "LeafScale.h"
#include "DrawableObject.h"
#include "MaterialManager.h"
#include "ObjectInteractionManager.h"
#include <vector>

/**
 * @brief Demo scéna - Shrek jazdí po Bézierovej spline
 *
 * Demonštruje:
 * - BezierSpline s viacerými segmentmi
 * - Tangent vector výpočet a rotácia podľa neho
 * - Materiály s ambient/diffuse/specular
 * - Mouse picking pre zadávanie bodov spline
 */
class Scene6_BezierSpline : public AbstractScene
{
private:
	BezierSpline* bezierSpline;
	LeafBezierMovement* bezierMovement;
	DrawableObject* shrekObject;
	MaterialManager* mm;
	ObjectInteractionManager* interactionManager;

	std::vector<DrawableObject*> controlPointMarkers;  // Vizuálne markery pre control pointy

	bool splineEditMode;  // Režim zadávania bodov
	bool shrekMoving;     // Či sa Shrek pohybuje

	// Debouncing pre klávesy
	bool keySpaceWasPressed;
	bool keyEWasPressed;
	bool keyRWasPressed;

	void createPredefinedSpline();  // Vytvor default spline
	void handleSplineInput();       // Mouse input pre body
	void handleKeyboardInput();     // Klavesnica pre ovládanie
	void addControlPointMarker(glm::vec3 position);  // Pridaj vizuálny marker

protected:
	void createShaders() override;
	void createDrawableObjects() override;
	void callbacks() override;

public:
	void createScene(GLFWwindow* window) override;
	void renderFrame() override;
	void renderScene() override;
};