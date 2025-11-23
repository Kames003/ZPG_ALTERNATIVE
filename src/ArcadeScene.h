#pragma once

#include "AbstractScene.h"
#include "Mole.h"
#include "Hammer.h"
#include "LoadedModel.h"
#include "SphereModel.h"
#include "SpotlightCamera.h"
#include <vector>
#include <map>

/**
 * @brief WHAC-A-MOLE - 3D Arkadova hra
 *
 * Herna scena kde:
 * - 6 podstavcov v mriežke 2x3
 * - Postavy (Shrek, Fiona, WC, Cat) vyskakuju nahodne
 * - Hrac klika myskou aby ich "udrel"
 * - Rozne postavy = rozne body
 * - Cat = penalta (nebit macku!)
 *
 * Ovladanie:
 * - MIDDLE CLICK: Udret postavu
 * - R: Restart hry
 * - ESC: Koniec
 */
class ArcadeScene : public AbstractScene
{
private:
    // Herny stav
    int score;
    int hits;
    int misses;
    float gameTime;
    float maxGameTime;
    bool gameOver;
    bool gamePaused;

    // Podstavce (6 kusov v 2x3 mriežke)
    static const int NUM_PEDESTALS = 6;
    static const int PEDESTAL_ROWS = 2;
    static const int PEDESTAL_COLS = 3;
    DrawableObject* pedestals[NUM_PEDESTALS];
    glm::vec3 pedestalPositions[NUM_PEDESTALS];
    bool pedestalOccupied[NUM_PEDESTALS];

    // Modely postav
    LoadedModel* shrekModel;
    LoadedModel* fionaModel;
    LoadedModel* toiletModel;
    LoadedModel* catModel;

    // Kladivo
    LoadedModel* hammerModel;
    Hammer* hammer;

    // Aktivne postavy (moles)
    std::vector<Mole*> moles;
    float spawnTimer;
    float spawnInterval;
    int maxActiveMoles;

    // Input
    bool keyRWasPressed;

    // Privatne metody (override z AbstractScene)
    void createShaders() override;
    void createDrawableObjects() override;

    // Vlastne metody
    void createLights();
    void createPedestals();
    void loadCharacterModels();

    Mole* spawnMole();
    Mole::Type getRandomMoleType();
    int getFreePedestal();

    void updateMoles(float deltaTime);
    void handleInput();
    void handleWhacking();
    void checkGameOver();
    void restartGame();

    void displayHUD();

public:
    ArcadeScene();
    ~ArcadeScene();

    // AbstractScene interface
    void createScene(GLFWwindow* window) override;
    void renderScene() override;
    void renderFrame() override;
    void callbacks() override;

    // Gettery pre HUD
    int getScore() const { return score; }
    int getHits() const { return hits; }
    int getMisses() const { return misses; }
    float getTimeRemaining() const { return maxGameTime - gameTime; }
    bool isGameOver() const { return gameOver; }
};