#pragma once

#include "ObjectManager.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ObjectInteractionManager
{
private:
    ObjectManager* om;
    Camera* camera;
    GLFWwindow* window;

public:

    ObjectInteractionManager(ObjectManager* om, Camera* camera, GLFWwindow* window);
    void placeObject(DrawableObject* obj, glm::vec3 worldPos);
    void removeObject(int stencilID);
    glm::vec3 screenToWorld(glm::vec3 screenPos);
};
