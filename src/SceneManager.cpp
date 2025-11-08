#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager(GLFWwindow* window)
{
    this->window = window;
    this->activeSceneIndex = -1;
    this->showHelp = false;

    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║      SCENE MANAGER INITIALIZED        ║\n");
    printf("╚═══════════════════════════════════════╝\n");
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

    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║   SWITCHING TO: %-22s║\n", scenes[index].name.c_str());
    printf("╚═══════════════════════════════════════╝\n");

    activeSceneIndex = index;

    // Lazy initialization
    if (!scenes[index].initialized)
    {
        printf("First time loading - initializing...\n\n");
        scenes[index].scene->createScene(window);
        scenes[index].initialized = true;
        printf("\nScene initialized successfully!\n");
    }
    else
    {
        printf("Scene already initialized - switching...\n");
    }

    printf("\n");
}

void SceneManager::renderActiveScene()
{
    if (activeSceneIndex < 0 || activeSceneIndex >= static_cast<int>(scenes.size()))
    {
        printf("ERROR: No active scene set!\n");
        return;
    }

    AbstractScene* scene = scenes[activeSceneIndex].scene;

    // Vykreslíme jeden frame aktívnej scény
    scene->renderFrame();

}

void SceneManager::toggleHelp()
{
    showHelp = !showHelp;
    
    // Vypíš help len ak sa práve zapol
    if (showHelp)
    {
        printf("\n");
        printf("╔═════════════════════════════════════════╗\n");
        printf("║       AVAILABLE SCENES:                 ║\n");
        printf("╠═════════════════════════════════════════╣\n");

        for (int i = 0; i < static_cast<int>(scenes.size()); i++)
        {
            const char* marker = (i == activeSceneIndex) ? "→" : " ";
            printf("║ %s [%d] %-33s║\n", marker, i + 1, scenes[i].name.c_str());
        }

        printf("╠═════════════════════════════════════════╣\n");
        printf("║ Controls:                               ║\n");
        printf("║  1-9    Switch to scene                 ║\n");
        printf("║  H      Toggle this help                ║\n");
        printf("║  ESC    Exit application                ║\n");
        printf("╚═════════════════════════════════════════╝\n\n");
    }
    else
    {
        printf("[Help hidden]\n");
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

SceneManager::~SceneManager()
{
    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║    CLEANING UP SCENE MANAGER          ║\n");
    printf("╚═══════════════════════════════════════╝\n");

    for (auto& entry : scenes)
    {
        if (entry.scene)
        {
            printf("Deleting scene: %s\n", entry.name.c_str());
            delete entry.scene;
        }
    }

    scenes.clear();
    printf("All scenes deleted\n\n");
}
