#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Subject.h"

#define VIEWMATRIX 0
#define PROJECTIONMATRIX 1

class Camera : public Subject
{
private:
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);

    GLFWwindow* window;
    float fov;
    float near;
    float far;
    float ratio = 0;

    int previousWidth = 0;
    int previousHeight = 0;
    int currentWidth = 0;
    int currentHeight = 0;

    glm::vec3 previousCameraPosition = glm::vec3(0.0f);
    glm::vec3 previousTarget = glm::vec3(0.0f);

    float speed = 0.05f;
    float sensitivity = 0.1f;

    bool firstClick = true;

    float alpha = 0.0f;
    float beta = 0.0f;

    double startX = 0;
    double startY = 0;

    double endX = 0;
    double endY = 0;

    double offsetX = 0;
    double offsetY = 0;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void calculateViewMatrix();
    void calculateProjectionMatrix();

    bool checkViewMatrixChanges();
    bool checkProjectionMatrixChanges();

    void notifyAll(int message);

public:
    Camera(GLFWwindow* w, float fov, float near, float far);

    void checkChanges();
    void controls();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();
    int getWidth();
    int getHeight();

    void setFOV(float newFOV); // for homework
    float getFOV() const { return fov; }


};

