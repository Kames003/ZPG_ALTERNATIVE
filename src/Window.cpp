#include "Window.h"

Window::Window(int width, int height, const char* name)
{
    // Vytvoríme GLFW okno
    this->w = glfwCreateWindow(width, height, name, NULL, NULL);

    // Kontrola či sa okno vytvorilo úspešne
    if (!w) {
        fprintf(stderr, "ERROR: Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    printf("Window created: %dx%d \"%s\"\n", width, height, name);
}

GLFWwindow* Window::getWindow()
{
    return this->w;
}