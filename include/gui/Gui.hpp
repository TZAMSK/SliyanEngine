#pragma once

struct GLFWwindow;
class Viewport;

#include "imgui.h"

class Gui
{
  public:
    void init(GLFWwindow *window);
    void beginFrame();
    void draw(Viewport &viewport);
    void endFrame();
    void shutdown();

    bool getAddShapeEnabled() const;
    void setAddShapeEnabled(bool value);

    void setShowAddShape(bool value);
    bool getShowAddShape() const;

    ImVec2 getViewportPos() const;
    ImVec2 getViewportSize() const;
    bool isMouseInsideViewport() const;

  private:
    void drawMenuBar();
    void drawToolbar();

    void drawLeftPanel();
    void drawRightPanel(Viewport &viewport);
    void drawViewport(Viewport &viewport);
    void drawConsole();

    void drawPopups();
    void drawAddShape();

  private:
    bool addShapeEnabled = false;
    bool showAddShapeWindow = false;
    bool showDemoWindow = false;

    ImVec2 viewportPos = ImVec2(0.0f, 0.0f);
    ImVec2 viewportSize = ImVec2(0.0f, 0.0f);
};
