#include "ObjectInteractionManager.h"
#include "DrawableObject.h"
#include <stdio.h>


ObjectInteractionManager::ObjectInteractionManager(ObjectManager* om, Camera* camera, GLFWwindow* window)
    : om(om), camera(camera), window(window)
{
    printf("ObjectInteractionManager created\n");
}


void ObjectInteractionManager::placeObject(DrawableObject* obj, glm::vec3 worldPos)
{
    if (!obj)
    {
        printf("ObjectInteractionManager::placeObject - object is nullptr\n");
        return;
    }

    obj->translate(worldPos);
    obj->scale(glm::vec3(0.25f));
    obj->calculateModelMatrix();
    om->addDrawableObject(obj);
    printf("Object placed at (%.2f, %.2f, %.2f)\n", worldPos.x, worldPos.y, worldPos.z);
}

void ObjectInteractionManager::removeObject(int stencilID)
{
    // Skybox has stencil ID 0, we dont want to get rid of the skybox :)
    if (stencilID == 0)
    {
        printf("Cannot remove skybox (stencil ID 0)\n");
        return;
    }
    int objectIndex = stencilID - 1;
    om->removeDrawableObject(objectIndex);
    printf("Object removed (stencil ID: %d, object index: %d)\n", stencilID, objectIndex);
}


glm::vec3 ObjectInteractionManager::screenToWorld(glm::vec3 screenPos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::vec4 viewport = glm::vec4(0, 0, width, height);

    glm::vec3 worldPos = glm::unProject(
        screenPos,
        camera->getViewMatrix(),
        camera->getProjectionMatrix(),
        viewport
    );

    printf("[screenToWorld] screen(%.1f, %.1f, %.4f) -> world(%.2f, %.2f, %.2f)\n",
           screenPos.x, screenPos.y, screenPos.z, worldPos.x, worldPos.y, worldPos.z);

    return worldPos;
}
