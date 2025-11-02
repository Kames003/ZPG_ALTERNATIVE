#include "Callback.h"
int x, y;

glm::vec3 Callback::position = glm::vec3(0.0f, 0.0f, 0.0f);
int Callback::clicked = 0;
int Callback::stencil_id;


Callback& Callback::GetInstance()
{
	static Callback* c = new Callback();
	return *c;
}

void Callback::window_size_callback(GLFWwindow* window, int width, int height)
{
	GetInstance().windowSizeCallback(window, width, height);
}

void Callback::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
}

void Callback::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	GetInstance().mouseButtonCallback(window, button, action, mode);
}

void Callback::mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		GLfloat depth;
		int newy = height - y - 10;
		glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		position = glm::vec3(x, newy, depth);
		clicked = 1;
	}

	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		GLuint index;
		int newy = height - y - 10;
		glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		stencil_id = index;
		clicked = 2;
	}
}

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetInstance().keyCallback(window, key, scancode, action, mods);
}

void Callback::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

void Callback::cursor_callback(GLFWwindow* window, double x, double y)
{
	GetInstance().cursorCallback(window, x, y);
}

void Callback::cursorCallback(GLFWwindow* window, double cx, double cy)
{
	x = cx; y = cy;
}

void Callback::window_iconify_callback(GLFWwindow* window, int iconified)
{
	GetInstance().windowIconifyCallback(window, iconified);
}

void Callback::windowIconifyCallback(GLFWwindow* window, int iconified)
{
	printf("window_iconify_callback \n");
}

void Callback::window_focus_callback(GLFWwindow* window, int focused)
{
	GetInstance().windowFocusCallback(window, focused);
}

void Callback::windowFocusCallback(GLFWwindow* window, int focused)
{
	printf("window_focus_callback \n");
}

void Callback::error_callback(int error, const char* description)
{
	GetInstance().errorCallback(error, description);
}

void Callback::errorCallback(int error, const char* description)
{
	fputs(description, stderr);
}
