#pragma once

class Application;
struct GLFWwindow;

class InputHandler
{
  public:
    void attach(GLFWwindow *window, Application *app);

  private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
};
