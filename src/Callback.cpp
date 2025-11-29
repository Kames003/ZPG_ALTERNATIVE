#include "Callback.h"
#include "SceneManager.h"

int x, y;

glm::vec3 Callback::position = glm::vec3(0.0f, 0.0f, 0.0f);
int Callback::clicked = 0;
int Callback::stencil_id;

Callback& Callback::GetInstance()
{
    static Callback* c = new Callback();
    return *c;
}


// HELPER METÓDY PRE MOUSE INTERAKCIU


bool Callback::hasLeftClick()
{
    return clicked == 1;
}

bool Callback::hasMiddleClick()
{
    return clicked == 2;
}

int Callback::getClickedObjectID()
{
    return stencil_id;
}

glm::vec3 Callback::getWorldPosition(GLFWwindow* window, Camera* camera)
{
    // Získaj rozmery okna
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::vec4 viewport = glm::vec4(0, 0, width, height);

    // Konvertuj screen space -> world space pomocou unProject
    return glm::unProject(
        position,
        camera->getViewMatrix(),
        camera->getProjectionMatrix(),
        viewport
    );
}

void Callback::clearClicks()
{
    clicked = 0;
}


// SCENE MANAGER INJECTION

void Callback::setSceneManager(SceneManager* sm)
{
    this->sceneManager = sm;
    printf("Callback: SceneManager injected\n");
}


// KEY CALLBACK - DELEGUJE NA handleSceneSwitching

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GetInstance().keyCallback(window, key, scancode, action, mods);
}

void Callback::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // ESC - zavri okno (globálna akcia)
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    // Scene switching (1-9, H)
    handleSceneSwitching(key, action);

    // Debug log
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}


// SCENE SWITCHING LOGIC

void Callback::handleSceneSwitching(int key, int action)
{
    if (!sceneManager)
    {
        // Žiadny scene manager - nemôžeme prepínať
        return;
    }

    int sceneCount = sceneManager->getSceneCount();


    // KLÁVESY 1-9 - PREPNI NA SCÉNU


    // Klávesa 1
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (!key1WasPressed && sceneCount >= 1)
        {
            sceneManager->setActiveScene(0);
            key1WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
    {
        key1WasPressed = false;
    }

    // Klávesa 2
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (!key2WasPressed && sceneCount >= 2)
        {
            sceneManager->setActiveScene(1);
            key2WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
    {
        key2WasPressed = false;
    }

    // Klávesa 3
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (!key3WasPressed && sceneCount >= 3)
        {
            sceneManager->setActiveScene(2);
            key3WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
    {
        key3WasPressed = false;
    }

    // Klávesa 4
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (!key4WasPressed && sceneCount >= 4)
        {
            sceneManager->setActiveScene(3);
            key4WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
    {
        key4WasPressed = false;
    }

    // Klávesa 5
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (!key5WasPressed && sceneCount >= 5)
        {
            sceneManager->setActiveScene(4);
            key5WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
    {
        key5WasPressed = false;
    }

    // Klávesa 6
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (!key6WasPressed && sceneCount >= 6)
        {
            sceneManager->setActiveScene(5);
            key6WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_RELEASE)
    {
        key6WasPressed = false;
    }

    // Klávesa 7
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (!key7WasPressed && sceneCount >= 7)
        {
            sceneManager->setActiveScene(6);
            key7WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_7 && action == GLFW_RELEASE)
    {
        key7WasPressed = false;
    }

    // Klávesa 8
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (!key8WasPressed && sceneCount >= 8)
        {
            sceneManager->setActiveScene(7);
            key8WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_8 && action == GLFW_RELEASE)
    {
        key8WasPressed = false;
    }

    // Klávesa 9
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (!key9WasPressed && sceneCount >= 9)
        {
            sceneManager->setActiveScene(8);
            key9WasPressed = true;
        }
    }
    else if (key == GLFW_KEY_9 && action == GLFW_RELEASE)
    {
        key9WasPressed = false;
    }


    // KLÁVESA H - TOGGLE HELP

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if (!keyHWasPressed)
        {
            sceneManager->toggleHelp();
            keyHWasPressed = true;
        }
    }
    else if (key == GLFW_KEY_H && action == GLFW_RELEASE)
    {
        keyHWasPressed = false;
    }
}


// OSTATNÉ CALLBACKS


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
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // Získaj window size (pre výpočet pozície)
        int winWidth, winHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);

        // Získaj framebuffer size (pre glReadPixels)
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        // Vypočítaj scale factor (pre Retina displeje)
        float scaleX = (float)fbWidth / (float)winWidth;
        float scaleY = (float)fbHeight / (float)winHeight;

        // Škáluj cursor position na framebuffer coordinates
        int fbX = (int)(x * scaleX);
        int fbY = (int)(y * scaleY);

        // Prepočítaj Y (OpenGL má origin dole-vľavo)
        int newy = fbHeight - fbY - 1;

        GLfloat depth;
        glReadPixels(fbX, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        // Ulož pozíciu v WINDOW coordinates (pre glm::unProject)
        int winNewY = winHeight - y - 1;
        position = glm::vec3(x, winNewY, depth);
        clicked = 1;

        printf("[Callback] LEFT CLICK: window(%d,%d) fb(%d,%d) depth=%.4f\n",
               (int)x, winNewY, fbX, newy, depth);
    }

    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        // Získaj window size
        int winWidth, winHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);

        // Získaj framebuffer size (pre glReadPixels)
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        // Vypočítaj scale factor (pre Retina displeje) shitty mac proste xd
        float scaleX = (float)fbWidth / (float)winWidth;
        float scaleY = (float)fbHeight / (float)winHeight;

        // Škáluj cursor position na framebuffer coordinates
        int fbX = (int)(x * scaleX);
        int fbY = (int)(y * scaleY);

        // Prepočítaj Y (OpenGL má origin dole-vľavo)
        int newy = fbHeight - fbY - 1;

        GLuint index;
        glReadPixels(fbX, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        stencil_id = index;
        clicked = 2;

        printf("[Callback] MIDDLE CLICK: fb(%d,%d) stencil_id=%d\n", fbX, newy, index);
    }
}

void Callback::cursor_callback(GLFWwindow* window, double x, double y)
{
    GetInstance().cursorCallback(window, x, y);
}

void Callback::cursorCallback(GLFWwindow* window, double cx, double cy)
{
    x = cx;
    y = cy;
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
