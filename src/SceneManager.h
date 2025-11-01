#pragma once

#include "AbstractScene.h"
#include <vector>
#include <string>

/**
 * @brief Manager pre správu a prepínanie scén
 * 
 * SceneManager riešim problém "odkomentuj a prekompiluj" tým, že:
 * - Drží zoznam všetkých dostupných scén
 * - Umožňuje prepínanie medzi scénami za behu (pomocou klávesnice)
 * - Spravuje životný cyklus scén (inicializácia, destroy)
 * - Zabezpečuje že len jedna scéna je aktívna
 * 
 * Workflow:
 * 1. Zaregistruj všetky scény: registerScene(new MinimalForestScene(), "Forest")
 * 2. Nastav prvú scénu: setActiveScene(0)
 * 3. V main loope volaj: renderActiveScene()
 * 4. Prepínaj scény klávesmi (1, 2, 3, ...)
 * 
 * Príklad použitia:
 * @code
 * SceneManager* sm = new SceneManager(window);
 * sm->registerScene(new MinimalForestScene(), "Minimal Forest");
 * sm->registerScene(new SphereScene(), "Sphere Test");
 * sm->setActiveScene(0);
 * 
 * while (!glfwWindowShouldClose(window)) {
 *     sm->renderActiveScene();
 * }
 * @endcode
 */
class SceneManager
{
private:
    /**
     * @brief Štruktúra pre uchovávaní scény + metadata
     */
    struct SceneEntry {
        AbstractScene* scene;   // Pointer na scénu
        std::string name;       // Názov scény (pre UI)
        bool initialized;       // Či je scéna už inicializovaná
    };

    std::vector<SceneEntry> scenes;  // Všetky registrované scény
    int activeSceneIndex;            // Index aktuálne aktívnej scény
    GLFWwindow* window;              // GLFW okno (potrebné pre inicializáciu scén)
    
    bool showHelp;                   // Či sa má zobrazovať help overlay
    
    /**
     * @brief Vyčistí aktuálnu scénu (voliteľné)
     */
    void cleanupCurrentScene();
    
    /**
     * @brief Vykreslí help overlay s nápovedou
     */
    void renderHelpOverlay();

public:
    /**
     * @brief Konštruktor
     * @param window GLFW okno
     */
    SceneManager(GLFWwindow* window);
    
    /**
     * @brief Zaregistruje novú scénu
     * 
     * @param scene Pointer na scénu (už alokovaný new)
     * @param name Názov scény (zobrazí sa v UI)
     * 
     * DÔLEŽITÉ: Scéna sa NEINICIALIZUJE hneď, len sa pridá do zoznamu.
     * Inicializácia prebehne pri prvom použití (lazy initialization).
     * 
     * Príklad:
     * @code
     * sm->registerScene(new MinimalForestScene(), "Minimal Forest");
     * sm->registerScene(new SphereScene(), "Sphere Test");
     * @endcode
     */
    void registerScene(AbstractScene* scene, const std::string& name);
    
    /**
     * @brief Nastaví aktívnu scénu
     * 
     * @param index Index scény (0-based)
     * 
     * Ak scéna ešte nie je inicializovaná, zavolá sa scene->createScene()
     * 
     * Príklad:
     * @code
     * sm->setActiveScene(0);  // Prvá scéna
     * sm->setActiveScene(1);  // Druhá scéna
     * @endcode
     */
    void setActiveScene(int index);
    
    /**
     * @brief Vykreslí aktuálne aktívnu scénu
     * 
     * Toto je hlavná metóda ktorá sa volá v main loope.
     * Namiesto while loopu v každej scéne, tento loop je tu centrálny.
     * 
     * Príklad:
     * @code
     * while (!glfwWindowShouldClose(window)) {
     *     sm->renderActiveScene();
     * }
     * @endcode
     */
    void renderActiveScene();
    
    /**
     * @brief Spracuje vstup z klávesnice (prepínanie scén)
     * 
     * Klávesy:
     * - 1, 2, 3, ... = Prepnutie na scénu 1, 2, 3, ...
     * - H = Toggle help overlay (zoznam scén)
     * - ESC = Zatvorenie aplikácie
     * 
     * Táto metóda sa volá automaticky v renderActiveScene()
     */
    void handleInput();
    
    /**
     * @brief Získa počet registrovaných scén
     */
    int getSceneCount() const { return static_cast<int>(scenes.size()); }
    
    /**
     * @brief Získa názov scény
     */
    std::string getSceneName(int index) const;
    
    /**
     * @brief Získa index aktívnej scény
     */
    int getActiveSceneIndex() const { return activeSceneIndex; }
    
    /**
     * @brief Destruktor - vyčistí všetky scény
     */
    ~SceneManager();
};