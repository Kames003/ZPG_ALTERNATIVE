#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Wrapper trieda pre GLFW okno
 *
 * Enkapsuluje vytvorenie a správu GLFW okna.
 * Zjednodušuje prácu s oknom - stačí vytvoriť objekt a je hotovo.
 *
 * Príklad použitia:
 * @code
 * Window* window = new Window(800, 600, "Moja aplikácia");
 * GLFWwindow* glfwWindow = window->getWindow();
 * @endcode
 */
class Window
{
private:
    /**
     * @brief GLFW okno
     *
     * Pointer na GLFW okno vytvorené pomocou glfwCreateWindow()
     */
    GLFWwindow* w;

public:
    /**
     * @brief Konštruktor - vytvorí GLFW okno
     *
     * @param width Šírka okna v pixeloch
     * @param height Výška okna v pixeloch
     * @param name Názov okna (zobrazí sa v titulku)
     *
     * Ak sa okno nepodarí vytvoriť, aplikácia sa ukončí.
     *
     * Príklad:
     * @code
     * Window* window = new Window(800, 600, "ZPG - OpenGL Project");
     * @endcode
     */
    Window(int width, int height, const char* name);

    /**
     * @brief Získa GLFW okno
     *
     * @return GLFWwindow* Pointer na GLFW okno
     *
     * Použitie:
     * @code
     * GLFWwindow* glfwWindow = window->getWindow();
     * glfwMakeContextCurrent(glfwWindow);
     * @endcode
     */
    GLFWwindow* getWindow();
};