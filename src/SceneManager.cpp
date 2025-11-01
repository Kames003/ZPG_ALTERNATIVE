#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager(GLFWwindow* window)
{
    this->window = window;
    this->activeSceneIndex = -1;
    this->showHelp = false;
}

void SceneManager::registerScene(AbstractScene* scene, const std::string& name)
{
    printf("Registering scene: %s\n", name.c_str());
    
    SceneEntry entry;
    entry.scene = scene;
    entry.name = name;
    entry.initialized = false;
    
    scenes.push_back(entry);
}

void SceneManager::setActiveScene(int index)
{
    if (index < 0 || index >= static_cast<int>(scenes.size()))
    {
        printf("ERROR: Invalid scene index %d (available: 0-%d)\n", 
               index, static_cast<int>(scenes.size()) - 1);
        return;
    }
    
    // Ak prepíname na inú scénu, môžeme vyčistiť predchádzajúcu
    // (zatiaľ necháme bez cleanup-u, pretože AbstractScene nemá cleanup metódu)
    
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║   SWITCHING TO SCENE: %-16s║\n", scenes[index].name.c_str());
    printf("╚═══════════════════════════════════════╝\n\n");
    
    activeSceneIndex = index;
    
    // Lazy initialization - inicializuj scénu len pri prvom použití
    if (!scenes[index].initialized)
    {
        printf("First time loading scene - initializing...\n");
        scenes[index].scene->createScene(window);
        scenes[index].initialized = true;
    }
}

void SceneManager::renderActiveScene()
{
    if (activeSceneIndex < 0 || activeSceneIndex >= static_cast<int>(scenes.size()))
    {
        printf("ERROR: No active scene set!\n");
        return;
    }
    
    AbstractScene* scene = scenes[activeSceneIndex].scene;
    
    // Vyčistime buffery
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Aktualizujeme kameru scény
    // POZNÁMKA: Musíme pristupovať cez protected členy - riešenie nižšie
    // scene->camera->checkChanges();
    // scene->camera->controls();
    
    // Vykreslíme scénu
    // POZNÁMKA: Musíme pristupovať cez protected členy
    // scene->om->drawObjects();
    
    // DOČASNÉ RIEŠENIE: Zavoláme renderScene() ktorá obsahuje celý loop
    // Problém: renderScene() má vlastný while loop, čo je konflikt
    // Riešenie: Buď upravíme AbstractScene, alebo použijeme iný prístup
    
    // Spracujeme vstup
    handleInput();
    
    // Vykreslíme help overlay ak je zapnutý
    if (showHelp)
    {
        renderHelpOverlay();
    }
    
    // Swap buffers a poll events
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void SceneManager::handleInput()
{
    // Prepínanie scén pomocou číselných klávesov 1-9
    for (int i = 0; i < std::min(9, static_cast<int>(scenes.size())); i++)
    {
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS)
        {
            // Debouncing - čakáme kým používateľ nepustí klávesu
            static int lastKey = -1;
            if (lastKey != (GLFW_KEY_1 + i))
            {
                setActiveScene(i);
                lastKey = GLFW_KEY_1 + i;
            }
        }
    }
    
    // Toggle help overlay
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        static bool lastHPressed = false;
        if (!lastHPressed)
        {
            showHelp = !showHelp;
            lastHPressed = true;
        }
    }
    else
    {
        static bool lastHPressed = false;
        lastHPressed = false;
    }
    
    // ESC - zatvorenie
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void SceneManager::renderHelpOverlay()
{
    // Tu by sme mohli vykresliť text overlay pomocou ImGui alebo custom renderovania
    // Zatiaľ len výpis do konzoly pri prvom zobrazení
    static bool firstTime = true;
    if (firstTime)
    {
        printf("\n╔═════════════════════════════════════════╗\n");
        printf("║          SCENE MANAGER HELP             ║\n");
        printf("╠═════════════════════════════════════════╣\n");
        printf("║ Available Scenes:                       ║\n");
        
        for (int i = 0; i < static_cast<int>(scenes.size()); i++)
        {
            printf("║  [%d] %-34s ║\n", i + 1, scenes[i].name.c_str());
        }
        
        printf("╠═════════════════════════════════════════╣\n");
        printf("║ Controls:                               ║\n");
        printf("║  1-9    - Switch to scene               ║\n");
        printf("║  H      - Toggle this help              ║\n");
        printf("║  ESC    - Exit application              ║\n");
        printf("╚═════════════════════════════════════════╝\n\n");
        
        firstTime = false;
    }
}

std::string SceneManager::getSceneName(int index) const
{
    if (index < 0 || index >= static_cast<int>(scenes.size()))
    {
        return "Invalid index";
    }
    return scenes[index].name;
}

void SceneManager::cleanupCurrentScene()
{
    // Placeholder pre budúce cleanup
    // AbstractScene by mal mať virtual void cleanup() metódu
}

SceneManager::~SceneManager()
{
    printf("\nCleaning up SceneManager...\n");
    
    // Vymažeme všetky scény
    for (auto& entry : scenes)
    {
        if (entry.scene)
        {
            printf("  Deleting scene: %s\n", entry.name.c_str());
            delete entry.scene;
        }
    }
    
    scenes.clear();
}