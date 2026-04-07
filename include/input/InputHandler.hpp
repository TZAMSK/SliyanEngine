#pragma once

#include "gui/Gui.hpp"
#include "scene/Scene.hpp"
#include "scene/camera/Camera.hpp"
#include <glm/glm.hpp>

class Application;
struct GLFWwindow;

class InputHandler
{
  public:
    void attach(GLFWwindow *window, Application *app);

  private:
    static glm::vec3 mouseLocalToWorldSpace(const Gui &gui, const Scene &scene, double mouseX, double mouseY);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

  private:
    Camera m_Camera;
    bool m_MiddleMouseHeld = false;
};
