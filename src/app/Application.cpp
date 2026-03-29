#include "app/Application.hpp"

#include "gui/Gui.hpp"
#include "viewport/Viewport.hpp"
#include "scene/commands/Shortcuts.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace
{
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    (void)mods;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double mouseX = 0.0;
        double mouseY = 0.0;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->handleMouseClick(mouseX, mouseY);
        }
    }
}
} // namespace

Gui *Application::getGui()
{
    return gui;
}

bool Application::init()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, "RzhavaRender", monitor, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, openShortcut);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    viewport = new Viewport();
    if (!viewport->init())
    {
        std::cout << "Failed to initialize viewport" << std::endl;
        return false;
    }

    gui = new Gui();
    gui->init(window);

    return true;
}

void Application::handleMouseClick(double mouseX, double mouseY)
{
    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);

    float ndcX = (2.0f * static_cast<float>(mouseX)) / static_cast<float>(width) - 1.0f;
    float ndcY = 1.0f - (2.0f * static_cast<float>(mouseY)) / static_cast<float>(height);

    std::cout << "Mouse clicked at screen: (" << mouseX << ", " << mouseY << ")\n";
    std::cout << "Converted to OpenGL: (" << ndcX << ", " << ndcY << ")\n";

    viewport->addTriangle(ndcX, ndcY);
}

void Application::loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        gui->beginFrame();

        viewport->renderScene();
        gui->draw(*viewport);

        gui->endFrame();

        glfwSwapBuffers(window);
    }
}

void Application::shutdown()
{
    if (gui)
    {
        gui->shutdown();
        delete gui;
        gui = nullptr;
    }

    if (viewport)
    {
        viewport->shutdown();
        delete viewport;
        viewport = nullptr;
    }

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

int Application::run()
{
    if (!init())
    {
        shutdown();
        return -1;
    }

    loop();
    shutdown();
    return 0;
}
