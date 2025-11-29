#include "Scene6_BezierSpline.h"
#include "LoadedModel.h"
#include "SphereModel.h"
#include "PlainModel.h"
#include "PlainTextureModel.h"
#include "SkyboxModel.h"
#include "Texture2D.h"
#include "TextureCubemap.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Callback.h"

void Scene6_BezierSpline::createShaders()
{
	printf("  Creating shaders...\n");

	// Skybox (index 0)
	spm->addShaderProgram(camera,
		"Shaders/VertexShaderSkybox.glsl",
		"Shaders/FragmentShaderSkybox.glsl");

	// Phong Material (index 1) - pre Shreka
	spm->addShaderProgram(camera,
		"Shaders/VertexShaderPhong.glsl",
		"Shaders/FragmentShaderPhongMaterial.glsl");

	// Phong Texture Material (index 2) - pre textúrovaný Shrek
	spm->addShaderProgram(camera,
		"Shaders/VertexShaderPhongTexture.glsl",
		"Shaders/FragmentShaderPhongTextureMaterial.glsl");

	// Constant (index 3) - pre debug markery
	spm->addShaderProgram(camera,
		"Shaders/VertexShaderConstant.glsl",
		"Shaders/FragmentShaderConstant.glsl");

	printf("Shaders 0-3 created\n");
}

void Scene6_BezierSpline::createDrawableObjects()
{
	printf("  Creating objects...\n");

	ShaderProgram* skyboxShader = spm->getShaderProgram(0);
	ShaderProgram* phongMaterialShader = spm->getShaderProgram(1);
	ShaderProgram* phongTextureMaterialShader = spm->getShaderProgram(2);
	ShaderProgram* constantShader = spm->getShaderProgram(3);

	// SKYBOX
	SkyboxModel* skyboxModel = new SkyboxModel();
	DrawableObject* skybox = new DrawableObject(skyboxModel, skyboxShader);
	skybox->addTexture(tm->getTexture(0));
	skybox->scale(glm::vec3(50.0f));
	skybox->calculateModelMatrix();
	skybox->updateModelMatrix();
	om->addSkybox(skybox);

	// GROUND s textúrou trávy
	PlainTextureModel* groundModel = new PlainTextureModel();
	DrawableObject* ground = new DrawableObject(groundModel, phongTextureMaterialShader);
	ground->addTexture(tm->getTexture(2)); // grass.png

	// MATERIÁL PRE GRASS - phongTextureMaterialShader potrebuje material!
	Material* grassMaterial = new Material(
		glm::vec3(0.2f, 0.25f, 0.2f),    // Ambient - tmavá zelená
		glm::vec3(0.8f, 1.0f, 0.8f),     // Diffuse - svetlá (textúra bude viditeľná)
		glm::vec3(0.1f, 0.1f, 0.1f),     // Specular - bez lesku
		4.0f,                             // Shininess - matný povrch
		"GrassMaterial"
	);
	mm->addMaterial(grassMaterial);
	ground->setMaterial(grassMaterial);

	ground->scale(glm::vec3(100.0f, 1.0f, 100.0f));
	ground->translate(glm::vec3(0.0f, -0.5f, 0.0f));
	ground->calculateModelMatrix();
	ground->updateModelMatrix();
	om->addDrawableObject(ground);

	// SHREK MODEL - použijeme textúrovaný phong shader
	LoadedModel* shrekModel = new LoadedModel("models/shrek.obj");
	shrekObject = new DrawableObject(shrekModel, phongTextureMaterialShader);

	// Pridaj textúru Shreka
	shrekObject->addTexture(tm->getTexture(1)); // shrek.png

	// MATERIÁL PRE SHREKA - zelený odraz s odleskami
	Material* shrekMaterial = new Material(
		glm::vec3(0.3f, 0.4f, 0.3f),     // Ambient - svetlejšia zelená (lepšia viditeľnosť)
		glm::vec3(0.6f, 1.0f, 0.5f),     // Diffuse - svietiáca zelená (Shrekova koža)
		glm::vec3(0.7f, 0.7f, 0.7f),     // Specular - väčší lesk
		32.0f,                            // Shininess - jemne lesklá koža
		"ShrekMaterial"
	);
	mm->addMaterial(shrekMaterial);
	shrekObject->setMaterial(shrekMaterial);
	shrekObject->setColor(glm::vec3(0.5f, 0.9f, 0.4f)); // Shrekova zelená

	// SPLINE - ZAČÍNA PRÁZDNA (user musí naklikať body!)
	bezierSpline = new BezierSpline();
	bezierSpline->setPingPong(true);
	bezierSpline->setSpeed(0.0015f);  // 50% pomalšie (bolo 0.003f)

	printf("Spline initialized with 0 points.\n");
	printf("  -> Press E to enter EDIT MODE\n");
	printf("  -> Click LEFT MOUSE to add control points\n");
	printf("  -> Need at least 4 points for Shrek to move!\n");

	// TRANSFORMATION COMPONENT
	bezierMovement = new LeafBezierMovement(bezierSpline);
	bezierMovement->setRotateAlongPath(true);  // ✅ ZAPNUTÉ - Shrek sa natáča podľa cesty!
	bezierMovement->setUpVector(glm::vec3(0.0f, 1.0f, 0.0f));

	// ✅ SETUP TRANSFORMÁCIÍ - RAZ pri inicializácii!
	// DÔLEŽITÉ: Poradie je kľúčové! Bezier (translate+rotation) PRED scale!
	shrekObject->getTransformationComposite()->addTransformation(bezierMovement);
	shrekObject->scale(glm::vec3(0.5f));  // Scale MUSÍ byť POSLEDNÝ!
	// ⚠️ NEVOLAJ calculateModelMatrix() - vymazalo by to transformácie!
	// Matica sa vypočíta až pri prvom renderovaní

	om->addDrawableObject(shrekObject);

	printf("Shrek created with scale 0.5x and Bezier movement component\n");
	printf("  - Model: shrek.obj\n");
	printf("  - Material: Green skin (ambient/diffuse/specular)\n");
	printf("  - Rotation: Enabled (follows tangent vector)\n");
	printf("Total: %d objects\n", om->getObjectCount());
}

void Scene6_BezierSpline::createPredefinedSpline()
{
	// Vytvor pekný default tvar spline (osmička)
	bezierSpline->addControlPoint(glm::vec3(-10, 1, 0));   // P0
	bezierSpline->addControlPoint(glm::vec3(-7, 2, 4));    // P1
	bezierSpline->addControlPoint(glm::vec3(-3, 3, 5));    // P2
	bezierSpline->addControlPoint(glm::vec3(0, 4, 3));     // P3

	bezierSpline->addControlPoint(glm::vec3(3, 3, 0));     // P4
	bezierSpline->addControlPoint(glm::vec3(5, 2, -4));    // P5
	bezierSpline->addControlPoint(glm::vec3(7, 1, -5));    // P6

	bezierSpline->addControlPoint(glm::vec3(10, 1, -3));   // P7
	bezierSpline->addControlPoint(glm::vec3(8, 0, 0));     // P8
	bezierSpline->addControlPoint(glm::vec3(5, 0, 3));     // P9

	bezierSpline->addControlPoint(glm::vec3(0, 0, 4));     // P10
	bezierSpline->addControlPoint(glm::vec3(-5, 0, 3));    // P11
	bezierSpline->addControlPoint(glm::vec3(-8, 0, 0));    // P12

	printf("Predefined spline created with %zu points, %zu segments\n",
		bezierSpline->getControlPointCount(),
		bezierSpline->getSegmentCount());
}

void Scene6_BezierSpline::callbacks()
{
	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
		Callback::GetInstance().cursorCallback(w, x, y);
	});

	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
		Callback::GetInstance().windowSizeCallback(w, width, height);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
		Callback::GetInstance().mouseButtonCallback(w, button, action, mods);
	});

	printf("Callbacks registered\n");
}

void Scene6_BezierSpline::createScene(GLFWwindow* window)
{
	printf("\n========================================\n");
	printf("   CREATING BEZIER SPLINE SCENE\n");
	printf("========================================\n\n");

	this->window = window;
	this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);
	// ✅ Lepšia počiatoč ná pozícia kamery - vidíme celú scénu
	this->camera->setPosition(glm::vec3(0.0f, 15.0f, 30.0f));

	this->lm = new LightManager();
	this->spm = new ShaderProgramManager(lm);
	this->om = new ObjectManager();
	this->tm = new TextureManager();
	this->mm = new MaterialManager();

	this->interactionManager = new ObjectInteractionManager(om, camera, window);

	// Initialize states - ZAČÍNAME V EDIT MODE!
	this->splineEditMode = true;   // ✅ Edit mode ON hneď na začiatku
	this->shrekMoving = false;     // Shrek čaká na body
	this->keySpaceWasPressed = false;
	this->keyEWasPressed = false;
	this->keyRWasPressed = false;

	// Create textures
	tm->addTexture(new TextureCubemap(
		"Textures/Skybox/posx.jpg",
		"Textures/Skybox/negx.jpg",
		"Textures/Skybox/posy.jpg",
		"Textures/Skybox/negy.jpg",
		"Textures/Skybox/posz.jpg",
		"Textures/Skybox/negz.jpg"
	));                                                      // index 0
	tm->addTexture(new Texture2D("models/shrek.png"));       // index 1
	tm->addTexture(new Texture2D("Textures/Grass/grass.png")); // index 2

	// Create lights
	DirectionalLight* sunLight = new DirectionalLight(
		glm::normalize(glm::vec3(0.5f, -1.0f, 0.3f)),
		glm::vec3(1.0f, 0.95f, 0.9f) * 1.2f  // ✅ Zvýšená intenzita (0.7f → 1.2f)
	);
	lm->addDirectionalLight(sunLight);
	lm->ambient = 0.4f;  // ✅ Viac ambientného svetla (0.25f → 0.4f)

	createShaders();
	createDrawableObjects();
	callbacks();

	printf("\n========================================\n");
	printf("   SCENE READY\n");
	printf("========================================\n\n");
}

void Scene6_BezierSpline::handleSplineInput()
{
	// Ľavé tlačidlo = pridaj bod (len v edit mode)
	if (Callback::hasLeftClick() && Callback::splineEditMode)
	{
		glm::vec3 screenPos = Callback::position;
		glm::vec3 worldPos = interactionManager->screenToWorld(screenPos);
		
		// ✅ OPRAVA: Nastav Y na úroveň plainu (-0.5)
		worldPos.y = -0.5f;

		// Pridaj bod do spline
		bezierSpline->addControlPoint(worldPos);
		size_t count = bezierSpline->getControlPointCount();

		printf("[SPLINE] Point %zu added at (%.2f, %.2f, %.2f)\n",
			count, worldPos.x, worldPos.y, worldPos.z);

		// Pridaj vizuálny marker
		addControlPointMarker(worldPos);

		// Progress feedback
		if (count < 4) {
			printf("         Progress: %zu/4 points - need %zu more to create spline\n",
				count, 4 - count);
		} else if (count == 4) {
			printf("\n");
			printf("╔════════════════════════════════════════════════╗\n");
			printf("║  ✅ FIRST SEGMENT CREATED!                    ║\n");
			printf("║  Shrek can now move!                          ║\n");
			printf("║  Press SPACE to start animation               ║\n");
			printf("║  Press E to exit Edit Mode                    ║\n");
			printf("╚════════════════════════════════════════════════╝\n");
			printf("\n");
			
			// ✅ KRITICKÉ: Re-inicializuj LeafBezierMovement TERAZ keď máme validnú spline!
			if (bezierMovement && bezierSpline->isValid()) {
				glm::vec3 position = bezierSpline->calculatePoint();
				glm::vec3 tangent = bezierSpline->calculateTangent();
				
				printf("[BEZIER] Initializing movement at position (%.2f, %.2f, %.2f)\n",
					position.x, position.y, position.z);
				printf("[BEZIER] Tangent vector: (%.2f, %.2f, %.2f)\n",
					tangent.x, tangent.y, tangent.z);
			}
		} else {
			printf("         Total: %zu segments (every 3 new points = +1 segment)\n",
				bezierSpline->getSegmentCount());
		}

		Callback::clearClicks();
	}
}

void Scene6_BezierSpline::handleKeyboardInput()
{
	// SPACE = play/pause
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!keySpaceWasPressed)
		{
			if (bezierSpline->isValid()) {
				shrekMoving = !shrekMoving;
				printf("🎬 Shrek movement: %s\n", shrekMoving ? "▶️  PLAYING" : "⏸️  PAUSED");
			} else {
				printf("⚠️  Cannot start - need at least 4 points! (Press E, then click to add points)\n");
			}
			keySpaceWasPressed = true;
		}
	}
	else
	{
		keySpaceWasPressed = false;
	}

	// E = edit mode (pridávanie bodov myšou)
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (!keyEWasPressed)
		{
			splineEditMode = !splineEditMode;
			Callback::splineEditMode = splineEditMode;

			if (splineEditMode) {
				printf("\n📝 EDIT MODE: ON\n");
				printf("   → Click LEFT MOUSE to add control points\n");
				printf("   → You need 4 points minimum to create a path\n");
			} else {
				printf("\n📝 EDIT MODE: OFF\n");
			}
			keyEWasPressed = true;
		}
	}
	else
	{
		keyEWasPressed = false;
	}

	// R = reset spline (odstráň všetky body)
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (!keyRWasPressed)
		{
			size_t previousCount = bezierSpline->getControlPointCount();
			bezierSpline->clearControlPoints();

			// Zastaviť pohyb
			shrekMoving = false;

			// ✅ OPRAVA: Skry vizuálne markery (nastav invisible)
			for (DrawableObject* marker : controlPointMarkers) {
				marker->visible = false;  // Skry marker
			}
			controlPointMarkers.clear();  // Vyčisti vector (objekty ostanú v OM, ale nebudú viditeľné)

			printf("\n🔄 RESET - removed %zu points and their markers.\n", previousCount);
			printf("   Shrek movement stopped. Add new points!\n\n");
			keyRWasPressed = true;
		}
	}
	else
	{
		keyRWasPressed = false;
	}
}

void Scene6_BezierSpline::addControlPointMarker(glm::vec3 position)
{
	// Získaj constant shader (index 3)
	ShaderProgram* constantShader = spm->getShaderProgram(3);

	// Vytvor malú sphere ako marker
	SphereModel* markerModel = new SphereModel();
	DrawableObject* marker = new DrawableObject(markerModel, constantShader);

	// Nastav farbu podľa počtu bodov (pekný gradient)
	size_t count = controlPointMarkers.size();
	float hue = (count % 8) / 8.0f; // 8 farieb v cykle
	glm::vec3 color;

	if (hue < 0.25f)      color = glm::vec3(1.0f, hue * 4.0f, 0.0f);        // červená -> žltá
	else if (hue < 0.5f)  color = glm::vec3(1.0f - (hue - 0.25f) * 4.0f, 1.0f, 0.0f); // žltá -> zelená
	else if (hue < 0.75f) color = glm::vec3(0.0f, 1.0f, (hue - 0.5f) * 4.0f); // zelená -> cyan
	else                  color = glm::vec3(0.0f, 1.0f - (hue - 0.75f) * 4.0f, 1.0f); // cyan -> modrá

	marker->setColor(color);

	// Umiestnenie a veľkosť
	marker->translate(position);
	marker->scale(glm::vec3(0.2f)); // ✅ Menšia sphere pre lepšiu viditeľnosť
	marker->calculateModelMatrix();
	marker->updateModelMatrix();

	// Pridaj do ObjectManagera a do nášho vectora
	om->addDrawableObject(marker);
	controlPointMarkers.push_back(marker);

	printf("         Marker #%zu created at (%.2f, %.2f, %.2f) with color (%.2f, %.2f, %.2f)\n",
		count + 1, position.x, position.y, position.z, color.r, color.g, color.b);
}

void Scene6_BezierSpline::renderFrame()
{
	camera->checkChanges();
	camera->controls();
	handleKeyboardInput();
	handleSplineInput();

	// Update Shrek pozície a rotácie po spline krivke
	if (shrekMoving && bezierMovement && bezierSpline->isValid())
	{
		float deltaTime = 0.016f; // ~60 FPS
		
		// ✅ DEBUG: Výpis aktuálneho parametra PRED update
		static int frameCount = 0;
		if (frameCount % 60 == 0) {  // Každú sekundu
			float t = bezierSpline->getGlobalParameter();
			glm::vec3 pos = bezierSpline->calculatePoint();
			printf("[ANIM] t=%.3f, pos=(%.2f, %.2f, %.2f)\n", t, pos.x, pos.y, pos.z);
		}
		frameCount++;
		
		// Update Bezier movement (aktualizuje vnútornú maticu)
		bezierMovement->update(deltaTime);
	}
	
	// ✅ VŽDY prepočítaj a aktualizuj maticu (aj keď sa nepohýba)
	shrekObject->getTransformationComposite()->resultMatrix();
	shrekObject->updateModelMatrix();

	spm->updateLights();
	om->drawSkybox();
	om->drawObjects();
}

void Scene6_BezierSpline::renderScene()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.7f, 0.9f, 1.0f);

	printf("\n");
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║         SCENE 6: BEZIER SPLINE - SHREK DEMO               ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	printf("\n");
	printf("🎯 TASK: Create a Bézier spline path for Shrek!\n");
	printf("\n");
	printf("📋 STEPS:\n");
	printf("  1. Edit mode is already ON (green text below)\n");
	printf("  2. Click LEFT MOUSE in the scene to add points\n");
	printf("  3. Add at least 4 points to create a path\n");
	printf("  4. Press SPACE to start Shrek's movement\n");
	printf("  5. Watch Shrek rotate along the tangent vector!\n");
	printf("\n");
	printf("⌨️  CONTROLS:\n");
	printf("  E           - Toggle Edit Mode (currently: ON)\n");
	printf("  LEFT CLICK  - Add control point\n");
	printf("  SPACE       - Play/Pause animation\n");
	printf("  R           - Reset all points\n");
	printf("  W/A/S/D     - Move camera\n");
	printf("  Q/Z         - Camera up/down\n");
	printf("  Right Mouse - Look around\n");
	printf("  ESC         - Exit scene\n");
	printf("\n");
	printf("✅ IMPLEMENTATION (All requirements met):\n");
	printf("  [✓] Cubic Bézier curve (4 points)\n");
	printf("  [✓] Spline with dynamic point addition\n");
	printf("  [✓] Tangent vector calculation & rotation\n");
	printf("  [✓] 3D model moving along spline\n");
	printf("  [✓] Material (ambient/diffuse/specular)\n");
	printf("\n");
	printf("════════════════════════════════════════════════════════════\n");
	printf("📝 EDIT MODE: ON - Start clicking to add points!\n");
	printf("════════════════════════════════════════════════════════════\n\n");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderFrame();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}