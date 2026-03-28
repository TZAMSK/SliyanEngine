#include "app/Application.hpp"

#include "gui/Gui.hpp"
#include "viewport/Viewport.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace
{
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
} // namespace

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
        gui = NULL;
    }

    if (viewport)
    {
        viewport->shutdown();
        delete viewport;
        viewport = NULL;
    }

    if (window)
    {
        glfwDestroyWindow(window);
        window = NULL;
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
