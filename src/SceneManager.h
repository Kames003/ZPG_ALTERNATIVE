// ========================================
// SceneManager.h - UPRAVENÁ VERZIA
// ========================================
#pragma once

#include "AbstractScene.h"
#include <vector>
#include <string>

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

public:
    SceneManager(GLFWwindow* window);

    void registerScene(AbstractScene* scene, const std::string& name);
    void setActiveScene(int index);
    void renderActiveScene();

    void toggleHelp();  

    int getSceneCount() const { return static_cast<int>(scenes.size()); }
    std::string getSceneName(int index) const;
    int getActiveSceneIndex() const { return activeSceneIndex; }

    ~SceneManager();
};