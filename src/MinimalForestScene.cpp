#include "MinimalForestScene.h"
#include "TreeModel.h"
#include "PlainModel.h"
#include "PlainTextureModel.h"
#include "DrawableObject.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotlightCamera.h"
#include "BenchModel.h"
#include "BushModel.h"
#include "SphereModel.h"
#include "Firefly.h"
#include "SkyboxModel.h"
#include <cstdlib>
#include "LoadedModel.h"
#include <ctime>
#include "MaterialManager.h"
#include "MTLLoader.h"
#include "Material.h"

void MinimalForestScene::createShaders()
{
    printf("  Creating shaders...\n");

    // SKYBOX shader (index 0)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderSkybox.glsl",
        "Shaders/FragmentShaderSkybox.glsl");

    // Phong shader (index 1)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    // Phong Texture shader (index 2) - pre textúrovanú zem
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTexture.glsl");

    // Constant shader pre fireflies (index 3)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderConstant.glsl",
        "Shaders/FragmentShaderConstant.glsl");

    // Phong Texture + Material shader (index 4) - pre textúry s materiálmi
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTexture.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");

    // Phong Material shader (index 5) - pre materiály BEZ textúry (ako Wolf)
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhong.glsl",
        "Shaders/FragmentShaderPhongMaterial.glsl");

    // === TEST SHADER (index 6) - Overenie homogennej zlozky w (s texturou) ===
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongTextureWTest.glsl",
        "Shaders/FragmentShaderPhongTextureMaterial.glsl");

    // === TEST SHADER (index 7) - Overenie homogennej zlozky w (BEZ textury) ===
    spm->addShaderProgram(camera,
        "Shaders/VertexShaderPhongWTest.glsl",
        "Shaders/FragmentShaderPhong.glsl");

    printf("Skybox shader (index 0)\n");
    printf("Phong shader (index 1) - 16 lights + flashlight\n");
    printf("Phong Texture shader (index 2) - textured ground\n");
    printf("Constant shader (index 3) - fireflies\n");
    printf("Phong Texture + Material shader (index 4) - texture + materials\n");
    printf("Phong Material shader (index 5) - materials without texture\n");
    printf("W-TEST Texture shader (index 6) - w=500 s texturou\n");
    printf("W-TEST shader (index 7) - w=500 BEZ textury\n");
}

void MinimalForestScene::createTextures()
{
    printf("  Creating textures...\n");
    
    // Cubemap pre skybox (index 0)
    tm->addTexture(new TextureCubemap(
        "Textures/Skybox/posx.jpg",
        "Textures/Skybox/negx.jpg",
        "Textures/Skybox/posy.jpg",
        "Textures/Skybox/negy.jpg",
        "Textures/Skybox/posz.jpg",
        "Textures/Skybox/negz.jpg"
    ));
    
    // Grass textúra (index 1)
    tm->addTexture(new Texture2D("Textures/Grass/grass.png"));

    // Shrek textúra (index 2) - PRESUNUŤ NA models/
    tm->addTexture(new Texture2D("models/shrek.png"));

    // Fiona textúra (index 3) - PRESUNUŤ NA models/
    tm->addTexture(new Texture2D("models/fiona.png"));

    // Toilet textúra (index 4) - PRESUNUŤ NA models/
    tm->addTexture(new Texture2D("models/toiled.jpg"));

    // Cat textúra (index 5)
    tm->addTexture(new Texture2D("models/Cat_diffuse.jpg"));

    printf("Skybox cubemap loaded (texture 0)\n");
    printf("Grass texture loaded (texture 1)\n");
    printf("Shrek texture loaded from models/ (texture 2)\n");
    printf("Fiona texture loaded from models/ (texture 3)\n");
    printf("Toilet texture loaded from models/ (texture 4)\n");
    printf("Cat texture loaded (texture 5)\n");
}

void MinimalForestScene::createMaterials()
{
    printf("  Creating materials...\n");

    // Načítaj materiály zo shrek.mtl
    MTLLoader::loadMTL("models/shrek.mtl", mm);

    // Načítaj materiály z Cat.mtl
    MTLLoader::loadMTL("models/Cat.mtl", mm);

    printf("Materials loaded successfully!\n");
}

void MinimalForestScene::createLights()
{
    printf("Starting to create lights...\n");

    DirectionalLight* moonLight = new DirectionalLight(
        glm::normalize(glm::vec3(0.3f, -1.0f, 0.2f)),
        glm::vec3(0.08f, 0.08f, 0.12f)
    );

    lm->addDirectionalLight(moonLight);
    lm->ambient = 0.10f;

    flashlight = new SpotlightCamera(
        1.0f,
        0.09f,
        0.032f,
        cos(glm::radians(12.5f)),
        glm::vec3(1.0f, 1.0f, 0.9f)
    );

    printf("Flashlight created (press F to toggle)\n");

    spm->updateLights();

    printf("DirectionalLight moonlight created\n");
    printf("Ambient of scene is : 0.20\n");
}

void MinimalForestScene::createDrawableObjects()
{
    printf("  Creating objects...\n");
    srand(static_cast<unsigned>(time(nullptr)));

    ShaderProgram* skyboxShader = spm->getShaderProgram(0);            // SKYBOX
    ShaderProgram* phongShader = spm->getShaderProgram(1);             // PHONG
    ShaderProgram* phongTextureShader = spm->getShaderProgram(2);      // PHONG TEXTURE
    ShaderProgram* constantShader = spm->getShaderProgram(3);          // CONSTANT
    ShaderProgram* phongTextureMaterialShader = spm->getShaderProgram(4); // PHONG TEXTURE + MATERIAL
    ShaderProgram* phongMaterialShader = spm->getShaderProgram(5);     // PHONG MATERIAL (bez textúry)
    ShaderProgram* wTestTextureShader = spm->getShaderProgram(6);      // W-TEST s texturou
    ShaderProgram* wTestShader = spm->getShaderProgram(7);             // W-TEST BEZ textury

    // ========================================
    // SKYBOX (renderuje sa ako prvý)
    // ========================================
    printf("  Creating skybox...\n");
    SkyboxModel* skyboxModel = new SkyboxModel();
    DrawableObject* skybox = new DrawableObject(skyboxModel, skyboxShader);
    skybox->addTexture(tm->getTexture(0)); // Cubemap texture
    skybox->scale(glm::vec3(50.0f)); // Veľká kocka okolo scény
    skybox->calculateModelMatrix();
    skybox->updateModelMatrix();
    om->addSkybox(skybox);
    printf("Skybox created\n");

    // ========== ZEMA S TEXTÚROU TRÁVY ==========
    PlainTextureModel* groundTextureModel = new PlainTextureModel();
    DrawableObject* ground = new DrawableObject(groundTextureModel, phongTextureShader);
    ground->addTexture(tm->getTexture(1)); // Grass textúra
    ground->scale(glm::vec3(100.0f, 1.0f, 100.0f));
    ground->translate(glm::vec3(0.0f, -0.5f, 0.0f));
    ground->calculateModelMatrix();
    ground->updateModelMatrix();
    om->addDrawableObject(ground);
    printf("Ground with grass texture created\n");

    // ========== CESTA (bez textúry) ==========
    PlainModel* pathModel = new PlainModel();
    DrawableObject* path = new DrawableObject(pathModel, phongShader);
    path->setColor(glm::vec3(0.6f, 0.5f, 0.4f));
    path->scale(glm::vec3(3.0f, 1.0f, 100.0f));
    path->translate(glm::vec3(0.0f, -0.4f, 0.0f));
    path->calculateModelMatrix();
    path->updateModelMatrix();
    om->addDrawableObject(path);

    // ========== STROMY ==========
    TreeModel* treeModel = new TreeModel();
    for (int i = 0; i < 70; i++)
    {
        float x, z;
        do {
            x = (rand() % 80) - 40.0f;
            z = (rand() % 80) - 40.0f;
        } while (abs(x) < 2.5f);

        float scale = 0.2f + static_cast<float>(rand() % 15) / 100.0f;
        float rotation = static_cast<float>(rand() % 360);

        DrawableObject* tree = new DrawableObject(treeModel, phongShader);
        tree->setColor(glm::vec3(0.1f, 0.2f, 0.05f));

        tree->translate(glm::vec3(x, -0.5f, z));
        tree->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        tree->scale(glm::vec3(scale));
        tree->calculateModelMatrix();
        tree->updateModelMatrix();
        om->addDrawableObject(tree);
    }
    printf("70 trees\n");

    // ========== LAVIČKY ==========
    BenchModel* benchModel = new BenchModel();
    float benchPositions[][3] = {
        {-5.0f, -0.5f, -10.0f},
        {5.0f, -0.5f, -10.0f},
        {-5.0f, -0.5f, 10.0f},
        {5.0f, -0.5f, 10.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        DrawableObject* bench = new DrawableObject(benchModel, phongShader);
        bench->setColor(glm::vec3(0.55f, 0.17f, 0.05f));
        bench->translate(glm::vec3(benchPositions[i][0],
                                   benchPositions[i][1],
                                   benchPositions[i][2]));
        bench->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        bench->scale(glm::vec3(1.5f));
        bench->calculateModelMatrix();
        bench->updateModelMatrix();
        om->addDrawableObject(bench);
    }
    printf("4 benches\n");

    // ========================================
    // LAMPY
    // ========================================
    printf("  Creating lamps (with increased light intensity for screenshot)...\n");
    LoadedModel* lampModel = new LoadedModel("models/lamp.obj");

    float lampPositions[][3] = {
        {-2.5f, 0.32f, -20.0f},
        {2.5f, 0.32f, -20.0f},
        {-2.5f, 0.32f, -10.0f},
        {2.5f, 0.32f, -10.0f},
        {-2.5f, 0.32f, 0.0f},
        {2.5f, 0.32f, 0.0f},
        {-2.5f, 0.32f, 10.0f},
        {2.5f, 0.32f, 10.0f},
        {-2.5f, 0.32f, 20.0f},
        {2.5f, 0.32f, 20.0f}
    };

    int numLamps = 10;

    for (int i = 0; i < numLamps; i++)
    {
        DrawableObject* lamp = new DrawableObject(lampModel, phongShader);
        lamp->setColor(glm::vec3(0.9f, 0.8f, 0.3f));
        lamp->translate(glm::vec3(lampPositions[i][0],
                                  lampPositions[i][1],
                                  lampPositions[i][2]));
        lamp->scale(glm::vec3(0.23f));
        lamp->calculateModelMatrix();
        lamp->updateModelMatrix();
        om->addDrawableObject(lamp);

        PointLight* lampLight = new PointLight(
            glm::vec3(lampPositions[i][0],
                     lampPositions[i][1] + 1.5f,
                     lampPositions[i][2]),
            1.0f,
            0.35f,
            0.8f,
            glm::vec3(1.0f, 0.9f, 0.7f) * 0.8f
        );
        lm->addPointLight(lampLight);
    }

    printf("%d lamps with INCREASED light intensity (for screenshot)\n", numLamps);

    // ========================================
    // SHUTTLE MODEL(HAVARUJÚCA LOĎKA)
    // ========================================
    printf("  Creating shuttle (crashing spaceship)...\n");
    LoadedModel* shuttleModel = new LoadedModel("models/shuttle.obj");

    DrawableObject* shuttle = new DrawableObject(shuttleModel, phongShader);
    shuttle->setColor(glm::vec3(0.7f, 0.7f, 0.75f));  // Kovová sivá farba

    // umiestnenie
    shuttle->translate(glm::vec3(-15.0f, 25.0f, 10.0f));  // Vľavo, vysoko, vpredu

    // Sklony pre efekt havarie
    shuttle->rotate(-25.0f, glm::vec3(1.0f, 0.0f, 0.0f));  // Sklon dopredu (nos dole)
    shuttle->rotate(15.0f, glm::vec3(0.0f, 0.0f, 1.0f));   // Mierne naklopenie do strany
    shuttle->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));   // Rotácia okolo Y (smer letu)

    shuttle->scale(glm::vec3(0.5f));  // Menšia veľkosť

    shuttle->calculateModelMatrix();
    shuttle->updateModelMatrix();
    om->addDrawableObject(shuttle);

    printf("Shuttle loaded - crashing above forest!\n");

    // ========================================
    // SHREK MODEL - DVA SHRECI S RÔZNYMI MATERIÁLMI + TEXTÚROU
    // ========================================
    printf("  Creating two Shreks with texture + different materials...\n");
    LoadedModel* shrekModel = new LoadedModel("models/shrek.obj");


    // === SHREK 1 - TESTOVACI s W=500 (s texturou) ===
    DrawableObject* shrek1 = new DrawableObject(shrekModel, wTestTextureShader);  // W-TEST TEXTURE SHADER!
    shrek1->addTexture(tm->getTexture(2)); // Shrek textúra


    Material* shrekMat1 = mm->getMaterialByName("Material.001");
    shrekMat1->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    shrekMat1->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekMat1->setSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
    shrekMat1->setShininess(1.0f);
    shrek1->setMaterial(shrekMat1);


    shrek1->translate(glm::vec3(-6.0f, -0.5f, -5.0f));
    shrek1->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek1->scale(glm::vec3(0.8f));

    shrek1->calculateModelMatrix();
    shrek1->updateModelMatrix();
    om->addDrawableObject(shrek1);

    printf("Shrek 1 - W-TEST TEXTURE (w=500 v shaderi, ocakavany vysledok: NORMALNA VELKOST)\n");

    // === SHREK 3 - TEST s LeafCustomW(20) cez Composite pattern ===
    DrawableObject* shrek3 = new DrawableObject(shrekModel, phongTextureMaterialShader);
    shrek3->addTexture(tm->getTexture(2)); // Shrek textura

    Material* shrekMat3 = new Material();
    shrekMat3->setName("ShrekCustomW");
    shrekMat3->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    shrekMat3->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    shrekMat3->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
    shrekMat3->setShininess(32.0f);
    mm->addMaterial(shrekMat3);
    shrek3->setMaterial(shrekMat3);

    // Vytvor custom maticu s [3][3] = 20 (objekt bude 20x mensi)
    glm::mat4 customIdentity = glm::mat4(1.0f);
    customIdentity[3][3] = 20.0f;

    // Umiestnenie - vedla Shrek1 (vlavo)
    // PORADIE JE DOLEZITE! customMatrix musi byt POSLEDNA (aplikuje sa na vertex PRVA)
    // Scale 20.0f -> efektivna velkost = 20/20 = 1.0 (normalna velkost pre test)
    shrek3->translate(glm::vec3(-8.0f, -0.5f, -5.0f));  // Tesne vlavo od Shrek1 (-6)
    shrek3->rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));  // Rovnaka rotacia ako Shrek1
    shrek3->scale(glm::vec3(0.8f));
    shrek3->customMatrix(customIdentity);  // VLASTNA TRANSFORMACIA - POSLEDNA!

    shrek3->calculateModelMatrix();
    shrek3->updateModelMatrix();
    om->addDrawableObject(shrek3);

    printf("Shrek 3 - CUSTOM MATRIX (LeafMatrix s [3][3]=20, ocakavany vysledok: 20x MENSI!)\n");

    DrawableObject* shrek2 = new DrawableObject(shrekModel, phongTextureMaterialShader);
    shrek2->addTexture(tm->getTexture(2));


    Material* shrekBronze = new Material();
    shrekBronze->setName("ShrekSuperShiny");
    shrekBronze->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));   // Mierny ambient
    shrekBronze->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));   // Plná textúra
    shrekBronze->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));  // MAXIMÁLNY biely lesk!
    shrekBronze->setShininess(256.0f);
    mm->addMaterial(shrekBronze);
    shrek2->setMaterial(shrekBronze);

    shrek2->translate(glm::vec3(10.0f, -0.5f, -8.0f));  // Ďalej vpravo a vzadu
    shrek2->rotate(-45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    shrek2->scale(glm::vec3(0.8f));

    shrek2->calculateModelMatrix();
    shrek2->updateModelMatrix();
    om->addDrawableObject(shrek2);

    printf("Shrek 2 - BRUTÁLNE LESKLÝ (specular: 1.0, shininess: 256) 💎✨\n");


    printf("  Creating Fiona...\n");
    LoadedModel* fionaModel = new LoadedModel("models/fiona.obj");
    
    DrawableObject* fiona = new DrawableObject(fionaModel, phongTextureShader);
    fiona->addTexture(tm->getTexture(3)); // Fiona textúra
    
    // Umiestnenie Fiony - vpravo od cesty, oproti Shrekovi
    fiona->translate(glm::vec3(6.0f, -0.5f, -5.0f));
    fiona->rotate(-30.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Otočená k Shrekovi
    fiona->scale(glm::vec3(0.75f)); // Trochu menšia ako Shrek
    
    fiona->calculateModelMatrix();
    fiona->updateModelMatrix();
    om->addDrawableObject(fiona);
    
    printf("Fiona loaded - standing opposite to Shrek!\n");

    // ========================================
    // TOILET MODEL
    // ========================================
    printf("  Creating Toilet...\n");
    LoadedModel* toiletModel = new LoadedModel("models/toiled.obj");
    
    DrawableObject* toilet = new DrawableObject(toiletModel, phongTextureShader);
    toilet->addTexture(tm->getTexture(4)); // Toilet textúra
    
    // Umiestnenie toalety - v lese vľavo vzadu, vtipná pozícia
    toilet->translate(glm::vec3(-12.0f, -0.5f, -18.0f));
    toilet->rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    toilet->scale(glm::vec3(0.5f)); // Podstatne menšia

    toilet->calculateModelMatrix();
    toilet->updateModelMatrix();
    om->addDrawableObject(toilet);

    printf("Toilet loaded - hidden in the forest!\n");

    printf("  Creating Cat on bench...\n");
    LoadedModel* catModel = new LoadedModel("models/Cat.obj");

    DrawableObject* cat = new DrawableObject(catModel, phongTextureMaterialShader);
    cat->addTexture(tm->getTexture(5)); // Cat textúra

    // Získaj materiál z Cat.mtl
    Material* catMat = mm->getMaterialByName("Cat");
    cat->setMaterial(catMat);

    cat->translate(glm::vec3(-5.0f, -0.06f, -10.0f));
    cat->rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    cat->rotate(360.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    cat->scale(glm::vec3(0.012f));
    cat->calculateModelMatrix();
    cat->updateModelMatrix();
    om->addDrawableObject(cat);

    printf("Cat loaded - sitting on bench\n");

    // ========== KRÍKY ==========
    BushModel* bushModel = new BushModel();
    for (int i = 0; i < 15; i++)
    {
        float x, z;
        do {
            x = (rand() % 60) - 30.0f;
            z = (rand() % 60) - 30.0f;
        } while (abs(x) < 3.0f);

        float scale = 0.8f + static_cast<float>(rand() % 200) / 1000.0f;
        float rotation = static_cast<float>(rand() % 360);

        DrawableObject* bush = new DrawableObject(bushModel, phongShader);
        bush->setColor(glm::vec3(0.2f, 0.35f, 0.15f));

        bush->translate(glm::vec3(x, -0.5f, z));
        bush->rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        bush->scale(glm::vec3(scale));
        bush->calculateModelMatrix();
        bush->updateModelMatrix();
        om->addDrawableObject(bush);
    }
    printf("15 bushes\n");

    // ========== SVETLUŠKY ==========
    printf("Creating fireflies...\n");
    SphereModel* sphereModel = new SphereModel();

    for (int i = 0; i < 12; i++)
    {
        float x, z;
        do {
            x = (rand() % 50) - 25.0f;
            z = (rand() % 50) - 25.0f;
        } while (abs(x) < 4.0f);

        float y = 1.0f + (rand() % 200) / 100.0f;

        glm::vec3 color;
        int colorType = rand() % 3;
        if (colorType == 0) {
            color = glm::vec3(1.0f, 0.9f, 0.5f);
        } else if (colorType == 1) {
            color = glm::vec3(0.7f, 1.0f, 0.6f);
        } else {
            color = glm::vec3(1.0f, 0.95f, 0.7f);
        }

        Firefly* firefly = new Firefly(
            glm::vec3(x, y, z),
            sphereModel,
            constantShader,
            color
        );

        lm->addPointLight(firefly->getLight());
        om->addDrawableObject(firefly->getVisual());
        fireflies.push_back(firefly);
    }

    printf("12 fireflies\n");
    printf("Total: %d objects\n", om->getObjectCount());
}

void MinimalForestScene::callbacks()
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        Callback::GetInstance().cursorCallback(w, x, y);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        Callback::GetInstance().windowSizeCallback(w, width, height);
    });

    // NOVÉ - registrácia mouse button callback pre picking
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        Callback::GetInstance().mouseButtonCallback(w, button, action, mods);
    });

    printf("Mouse callbacks registered:\n");
    printf("  - LEFT CLICK: Place tree at cursor position\n");
    printf("  - MIDDLE CLICK: Remove clicked object\n");
}

void MinimalForestScene::createScene(GLFWwindow* window)
{
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║   CREATING FOREST SCENE               ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

    this->window = window;
    this->camera = new Camera(window, 60.0f, 0.1f, 200.0f);

    this->lm = new LightManager();
    this->spm = new ShaderProgramManager(lm);
    this->om = new ObjectManager();
    this->tm = new TextureManager();
    this->mm = new MaterialManager();  // MaterialManager

    // Initialize ObjectInteractionManager for object manipulation
    this->interactionManager = new ObjectInteractionManager(om, camera, window);

    createShaders();
    createTextures();
    createMaterials();  // Načítanie materiálov
    createLights();
    createDrawableObjects();
    callbacks();

    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║         SCENE READY                   ║\n");
    printf("║  Shrek 1: Normal shine (left)        ║\n");
    printf("║  Shrek 2: BRUTAL SHINE (right) 💎     ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
}

void MinimalForestScene::handleFlashlightInput()
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!keyFWasPressed)
        {
            flashlightEnabled = !flashlightEnabled;

            if (flashlightEnabled)
            {
                lm->addSpotlightCamera(flashlight);
                printf("\nFlashlight ON\n");
            }
            else
            {
                lm->removeSpotlightCamera();
                printf("\nFlashlight OFF\n");
            }

            spm->updateLights();
            keyFWasPressed = true;
        }
    }
    else
    {
        keyFWasPressed = false;
    }
}

void MinimalForestScene::renderFrame()
{
    camera->checkChanges(); // skontroluj či sa zmenila pozicia, rotacia, fov
    camera->controls(); // spracuj wasd, myš, space/shift

    handleFlashlightInput(); // skontroluj klavesu f pre baterku

    // Spracuj mouse input (picking, placing) cez Callback API
    handleMouseInput();

    float time = static_cast<float>(glfwGetTime()); // ziskaj aktualny čas
    for (Firefly* firefly : fireflies)
    {
        firefly->updateAnimation(time); // pohybuj svetluškami
    }

    spm->updateLights(); // pošli info o svetlach do shaderov

    om->drawSkybox();    // NOVÉ: vykreslí skybox NAJPRV
    om->drawObjects();   // vykresli všetky objekty
} // end :)

void MinimalForestScene::renderScene()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    // NOVÉ - enable stencil buffer pre picking
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    printf("╔═════════════════════════════════════════╗\n");
    printf("║       RENDER LOOP                     ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║ W/A/S/D     - Move                    ║\n");
    printf("║ Space/Shift - Up/Down                 ║\n");
    printf("║ Right Mouse - Look                    ║\n");
    printf("║ F           - Flashlight ON/OFF       ║\n");
    printf("║ LEFT CLICK  - Place tree              ║\n");
    printf("║ MIDDLE CLICK- Remove object           ║\n");
    printf("║ ESC         - Exit                    ║\n");
    printf("╠═════════════════════════════════════════╣\n");

    // už prakticky nepouzivame delegovali sme to do application loopy

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        renderFrame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// ========================================
// MOUSE INTERACTION - PICKING & PLACING
// ========================================

void MinimalForestScene::handleMouseInput()
{
    // Ľavé tlačidlo - umiestni strom (cez ObjectInteractionManager)
    if (Callback::hasLeftClick())
    {
        glm::vec3 screenPos = Callback::position;
        glm::vec3 worldPos = interactionManager->screenToWorld(screenPos);
        placeTreeAt(worldPos);
        Callback::clearClicks();
    }

    // Stredné tlačidlo - zmaž objekt (cez ObjectInteractionManager)
    if (Callback::hasMiddleClick())
    {
        int stencilID = Callback::getClickedObjectID();
        removeObjectAt(stencilID);
        Callback::clearClicks();
    }
}

void MinimalForestScene::placeTreeAt(glm::vec3 worldPos)
{
    printf("\n[MinimalForestScene] LEFT CLICK - Placing tree\n");
    printf("  World position: (%.2f, %.2f, %.2f)\n", worldPos.x, worldPos.y, worldPos.z);

    TreeModel* newTreeModel = new TreeModel();
    ShaderProgram* phongShader = spm->getShaderProgram(1);

    DrawableObject* newTree = new DrawableObject(newTreeModel, phongShader);
    newTree->setColor(glm::vec3(0.1f, 0.3f, 0.05f));

    // Deleguj umiestnenie na ObjectInteractionManager
    interactionManager->placeObject(newTree, worldPos);

    printf("  ✓ Tree placed! Total objects: %d\n", om->getObjectCount());
}

void MinimalForestScene::removeObjectAt(int stencilID)
{
    printf("\n[MinimalForestScene] MIDDLE CLICK - Object picking\n");
    printf("  Stencil ID: %d\n", stencilID);

    // Deleguj mazanie na ObjectInteractionManager
    interactionManager->removeObject(stencilID);
}