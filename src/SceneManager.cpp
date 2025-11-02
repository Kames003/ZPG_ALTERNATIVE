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

    // Spracujeme vstup (prepínanie scén)
    handleInput();

    // Help overlay
    if (showHelp)
    {
        static bool firstShow = true;
        if (firstShow)
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

            firstShow = false;
        }
    }
}

void SceneManager::handleInput()
{
    // Klávesa 1
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (!key1WasPressed && scenes.size() >= 1)
        {
            setActiveScene(0);
            key1WasPressed = true;
        }
    }
    else
    {
        key1WasPressed = false;
    }

    // Klávesa 2
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (!key2WasPressed && scenes.size() >= 2)
        {
            setActiveScene(1);
            key2WasPressed = true;
        }
    }
    else
    {
        key2WasPressed = false;
    }

    // Klávesa 3
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (!key3WasPressed && scenes.size() >= 3)
        {
            setActiveScene(2);
            key3WasPressed = true;
        }
    }
    else
    {
        key3WasPressed = false;
    }

    // Klávesa 4
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (!key4WasPressed && scenes.size() >= 4)
        {
            setActiveScene(3);
            key4WasPressed = true;
        }
    }
    else
    {
        key4WasPressed = false;
    }

    // Klávesa 5
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (!key5WasPressed && scenes.size() >= 5)
        {
            setActiveScene(4);
            key5WasPressed = true;
        }
    }
    else
    {
        key5WasPressed = false;
    }

    // Klávesa 6
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (!key6WasPressed && scenes.size() >= 6)
        {
            setActiveScene(5);
            key6WasPressed = true;
        }
    }
    else
    {
        key6WasPressed = false;
    }

    // Klávesa 7
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        if (!key7WasPressed && scenes.size() >= 7)
        {
            setActiveScene(6);
            key7WasPressed = true;
        }
    }
    else
    {
        key7WasPressed = false;
    }

    // Klávesa 8
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        if (!key8WasPressed && scenes.size() >= 8)
        {
            setActiveScene(7);
            key8WasPressed = true;
        }
    }
    else
    {
        key8WasPressed = false;
    }

    // Klávesa 9
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        if (!key9WasPressed && scenes.size() >= 9)
        {
            setActiveScene(8);
            key9WasPressed = true;
        }
    }
    else
    {
        key9WasPressed = false;
    }

    // Toggle help
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        if (!keyHWasPressed)
        {
            showHelp = !showHelp;
            keyHWasPressed = true;
        }
    }
    else
    {
        keyHWasPressed = false;
    }

    // ESC - exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
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