#include "input/InputHandler.hpp"

#include "app/Application.hpp"
#include "gui/Gui.hpp"
#include "scene/commands/CommandId.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void InputHandler::attach(GLFWwindow *window, Application *app)
{
    glfwSetWindowUserPointer(window, app);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;

    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app || action != GLFW_PRESS)
    {
        return;
    }

    if (key == GLFW_KEY_ESCAPE)
    {
        app->requestClose();
        return;
    }

    if ((mods & GLFW_MOD_CONTROL) && key == GLFW_KEY_A)
    {
        app->executeCommand(CommandId::OpenAddShapeDialog);
        return;
    }

    if (key == GLFW_KEY_1)
    {
        app->executeCommand(CommandId::CameraIsometric);
    }
    else if (key == GLFW_KEY_2)
    {
        app->executeCommand(CommandId::CameraTop);
    }
    else if (key == GLFW_KEY_3)
    {
        app->executeCommand(CommandId::CameraFront);
    }
    else if (key == GLFW_KEY_4)
    {
        app->executeCommand(CommandId::CameraSide);
    }
}

void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    (void)mods;

    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app || button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
    {
        return;
    }

    if (!app->getGui().isTrianglePlacementArmed())
    {
        return;
    }

    if (!app->getGui().isMouseInsideViewport())
    {
        return;
    }

    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    app->onViewportClicked(mouseX, mouseY);
    app->executeCommand(CommandId::CloseAddShapeDialog);
}
