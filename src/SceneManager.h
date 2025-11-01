#pragma once

#include "AbstractScene.h"
#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief Manager pre správu a prepínanie scén
 */
class SceneManager
{
private:
    struct SceneEntry {
        AbstractScene* scene;
        std::string name;
        bool initialized;
    };

    std::vector<SceneEntry> scenes;
    int activeSceneIndex;
    GLFWwindow* window;
    bool showHelp;

    // Debouncing pre klávesy
    bool key1WasPressed = false;
    bool key2WasPressed = false;
    bool key3WasPressed = false;
    bool key4WasPressed = false;
    bool key5WasPressed = false;
    bool key6WasPressed = false;
    bool key7WasPressed = false;
    bool key8WasPressed = false;
    bool key9WasPressed = false;
    bool keyHWasPressed = false;

public:
    SceneManager(GLFWwindow* window);

    void registerScene(AbstractScene* scene, const std::string& name);
    void setActiveScene(int index);
    void renderActiveScene();
    void handleInput();

    int getSceneCount() const { return static_cast<int>(scenes.size()); }
    std::string getSceneName(int index) const;
    int getActiveSceneIndex() const { return activeSceneIndex; }

    ~SceneManager();
};