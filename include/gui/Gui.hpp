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

    void drawAddShape();
    void setShowAddShape(bool value);

  private:
    void drawMenuBar();
    void drawToolbar();
    void drawColorPicker(Viewport &viewport);
    void drawInspector();
    void drawConsole();

  private:
    bool showDemoWindow = true;
    bool showAddShapeWindow = false;
};
