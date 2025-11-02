#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Wrapper trieda pre GLFW okno
 *
 */
class Window
{
private:

      // Pointer na GLFW okno vytvorené pomocou glfwCreateWindow()

    GLFWwindow* w;

public:

    Window(int width, int height, const char* name);

    GLFWwindow* getWindow();
};