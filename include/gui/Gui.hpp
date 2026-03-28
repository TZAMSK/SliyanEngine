#pragma once

struct GLFWwindow;
class Viewport;

class Gui
{
  public:
    void init(GLFWwindow *window);
    void beginFrame();
    void draw(Viewport &viewport);
    void endFrame();
    void shutdown();

  private:
    void drawMenuBar();
    void drawToolbar();
    void drawInspector();
    void drawConsole();

  private:
    bool showDemoWindow = true;
};
