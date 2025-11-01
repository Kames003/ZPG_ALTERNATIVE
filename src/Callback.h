#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h> 

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "Camera.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include "ObjectManager.h"


class Callback
{
public:
    static glm::vec3 position;
    static int clicked;
    static int stencil_id;
    static Callback& GetInstance();

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
};

