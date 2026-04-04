#include "input/InputHandler.hpp"

#include "app/Application.hpp"
#include "gui/Gui.hpp"
#include "scene/commands/CommandId.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

#include "ImGuizmo.h"

void InputHandler::attach(GLFWwindow *window, Application *app)
{
    glfwSetWindowUserPointer(window, app);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
}

glm::vec3 InputHandler::mouseLocalToWorldSpace(const Gui &gui, const Scene &scene, double mouseX, double mouseY)
{
    const ImVec2 viewportPos = gui.getViewportPos();
    const ImVec2 viewportSize = gui.getViewportSize();

    if (viewportSize.x <= 0.0f || viewportSize.y <= 0.0f)
        return glm::vec3(0.0f);

    const float localX = static_cast<float>(mouseX) - viewportPos.x;
    const float localY = static_cast<float>(mouseY) - viewportPos.y;

    const float ndcX = (2.0f * localX) / viewportSize.x - 1.0f;
    const float ndcY = 1.0f - (2.0f * localY) / viewportSize.y;

    const glm::mat4 projection = scene.getCamera().getProjection(viewportSize);
    const glm::mat4 view = scene.getCamera().getLookAt();

    const glm::mat4 inverseVP = glm::inverse(projection * view);
    const glm::vec4 nearPoint = inverseVP * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    const glm::vec4 farPoint = inverseVP * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    const glm::vec3 rayOrigin = glm::vec3(nearPoint) / nearPoint.w;
    const glm::vec3 rayEnd = glm::vec3(farPoint) / farPoint.w;
    const glm::vec3 rayDirection = glm::normalize(rayEnd - rayOrigin);

    if (glm::abs(rayDirection.z) < 0.0001f)
        return glm::vec3(0.0f);

    const float t = -rayOrigin.z / rayDirection.z;
    return rayOrigin + rayDirection * t;
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;

    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app || action != GLFW_PRESS)
        return;

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
        app->executeCommand(CommandId::CameraIsometric);
    else if (key == GLFW_KEY_2)
        app->executeCommand(CommandId::CameraTop);
    else if (key == GLFW_KEY_3)
        app->executeCommand(CommandId::CameraFront);
    else if (key == GLFW_KEY_4)
        app->executeCommand(CommandId::CameraSide);
}

void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    (void)mods;

    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app)
        return;

    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            app->getInputHandler().middleMouseHeld = true;
            app->getScene().getCamera().resetFirstMouse();
        }
        else if (action == GLFW_RELEASE)
        {
            app->getInputHandler().middleMouseHeld = false;
        }
        return;
    }

    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    if (app->getGui().isMouseInsideGizmoPanel())
        return;

    if (ImGuizmo::IsOver() || ImGuizmo::IsUsing())
        return;

    if (!app->getGui().isMouseInsideViewport())
        return;

    const glm::vec3 worldPoint = mouseLocalToWorldSpace(app->getGui(), app->getScene(), mouseX, mouseY);

    if (app->getGui().isAnyPlacementArmed())
    {
        app->onViewportClicked(worldPoint);
        return;
    }

    if (app->getSelectionManager().hoveredId() != 0)
        app->getSelectionManager().clickSelect();
    else
        app->getSelectionManager().clearSelection();
}

void InputHandler::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app)
        return;

    if (!app->getGui().isMouseInsideViewport())
    {
        app->getScene().getCamera().resetFirstMouse();
    }

    if (app->getInputHandler().middleMouseHeld)
    {
        app->getScene().getCamera().moveCamera(xpos, ypos);
    }

    app->getSelectionManager().updateHover(xpos, ypos, app->getGui(), app->getRenderer());
}

void InputHandler::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    (void)xoffset;

    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app)
        return;

    if (!app->getGui().isMouseInsideViewport())
        return;

    app->getScene().getCamera().zoomCamera(yoffset);
}
