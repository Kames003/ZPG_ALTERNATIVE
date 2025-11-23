#include "Camera.h"
#include "Observer.h"
#include <iostream>

bool Camera::checkViewMatrixChanges()
{
	if (previousCameraPosition != cameraPosition || previousTarget != target)
	{
		// zmena
		previousCameraPosition = cameraPosition;
		previousTarget = target;
		calculateViewMatrix(); // prepočítaj

		return true;
	}
	return false;
}

bool Camera::checkProjectionMatrixChanges()
{
	glfwGetWindowSize(window, &currentWidth, &currentHeight);

	if (currentWidth != previousWidth || currentHeight != previousHeight)
	{
		ratio = float(currentWidth) / float(currentHeight);
		previousWidth = currentWidth;
		previousHeight = currentHeight;
		calculateProjectionMatrix();

		return true;
	}

	return false;
}

void Camera::notifyAll(int message) // message = VIEWMATRIX ( koštanta 0 )
{
	for (Observer* o : observerCollection)
	{
		o->notify(message); // buď 0 alebo 1 a podla toho rozhodneem
	}
}

Camera::Camera(GLFWwindow* w, float fov, float near, float far)
{
	this->window = w;
	this->fov = fov;
	this->near = near;
	this->far = far;
}


void Camera::calculateViewMatrix()
{
	this->viewMatrix = glm::lookAt(cameraPosition, cameraPosition + target, up);
}

void Camera::calculateProjectionMatrix()
{
	this->projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
}

void Camera::checkChanges()
{
	if (checkViewMatrixChanges())
	{
		notifyAll(VIEWMATRIX);
	}

	if (checkProjectionMatrixChanges())
	{
		notifyAll(PROJECTIONMATRIX);
	}
}

void Camera::controls()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition += speed * target;
		printf("Camera position: (%.2f, %.2f, %.2f) [this=%p]\n",
			  cameraPosition.x, cameraPosition.y, cameraPosition.z,
			  (void*)this);  // Adresa kamery v pamäti!
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition -= speed * target;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition -= speed * glm::normalize(glm::cross(target, up));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition += speed * glm::normalize(glm::cross(target, up));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraPosition += speed * up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraPosition -= speed * up;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			startX = currentWidth / 2;
			startY = currentHeight / 2;
			glfwSetCursorPos(window, startX, startY);
			firstClick = false;
		}

		glfwGetCursorPos(window, &endX, &endY);

		offsetX = endX - startX;
		offsetY = startY - endY;

		startX = endX;
		startY = endY;

		offsetX *= sensitivity;
		offsetY *= sensitivity;

		alpha += offsetX;
		beta += offsetY;

		if (beta > 89.9f)
			beta = 89.9f;
		if (beta < -89.9f)
			beta = -89.9f;

		target.x = sin(glm::radians(alpha)) * cos(glm::radians(beta));
		target.y = sin(glm::radians(beta));
		target.z = cos(glm::radians(alpha)) * -cos(glm::radians(beta));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		firstClick = true;
	}
}

glm::mat4 Camera::getViewMatrix()
{
	return this->viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

glm::vec3 Camera::getCameraPosition()
{
	return this->cameraPosition;
}

glm::vec3 Camera::getCameraDirection()
{
	return this->target;
}

// taks cviko 6

void Camera::setFOV(float newFOV)
{
	this->fov = newFOV;
	calculateProjectionMatrix();  // Prepočítaj projection matrix
	notifyAll(PROJECTIONMATRIX);     // Notifikuj observerov (shadery)
}

//int Camera::getWidth()
//{
//	return this->currentWidth;
//}
//
//int Camera::getHeight()
//{
//	return this->currentHeight;
//}

void Camera::setPosition(glm::vec3 pos)
{
    this->cameraPosition = pos;
    calculateViewMatrix();
    notifyAll(VIEWMATRIX);
}

void Camera::setTarget(glm::vec3 tgt)
{
    // Target je smer kamery, nie bod
    // Vypocitaj smer z pozicie ku cielu
    this->target = glm::normalize(tgt - cameraPosition);
    calculateViewMatrix();
    notifyAll(VIEWMATRIX);
}