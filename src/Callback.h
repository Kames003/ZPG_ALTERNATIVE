#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include "Camera.h"

// Forward declaration
class SceneManager;

class Callback
{
private:
    // Scene switching
    SceneManager* sceneManager = nullptr;

    // Debouncing pre scene switching klávesy
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
    static glm::vec3 position;
    static int clicked;
    static int stencil_id;

    static Callback& GetInstance();


    // HELPER METÓDY PRE MOUSE INTERAKCIU


    static bool hasLeftClick();
    static bool hasMiddleClick();
    static int getClickedObjectID();
    static glm::vec3 getWorldPosition(GLFWwindow* window, Camera* camera);
    static void clearClicks();


    // SCENE MANAGER INJECTION

    void setSceneManager(SceneManager* sm);


    // CALLBACK METHODS

    static void window_size_callback(GLFWwindow* window, int width, int height);
    void windowSizeCallback(GLFWwindow* window, int width, int height);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void cursor_callback(GLFWwindow* window, double x, double y);
    void cursorCallback(GLFWwindow* window, double x, double y);

    static void window_iconify_callback(GLFWwindow* window, int iconified);
    void windowIconifyCallback(GLFWwindow* window, int iconified);

    static void window_focus_callback(GLFWwindow* window, int focused);
    void windowFocusCallback(GLFWwindow* window, int focused);

    static void error_callback(int error, const char* description);
    void errorCallback(int error, const char* description);

private:

    // SCENE SWITCHING LOGIC

    void handleSceneSwitching(int key, int action);
};
