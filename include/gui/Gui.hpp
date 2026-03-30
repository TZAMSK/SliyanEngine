#pragma once

#include <memory>
#include "imgui.h"

struct GLFWwindow;
class Viewport;
class Message;

class Gui
{
  public:
    Gui();
    ~Gui();

    void init(GLFWwindow *window);
    void beginFrame();
    void draw(Viewport &viewport);
    void endFrame();
    void shutdown();

    Message &getMessage();
    const Message &getMessage() const;

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
    std::unique_ptr<Message> message;

    bool addShapeEnabled = false;
    bool showAddShapeWindow = false;
    bool showDemoWindow = false;

    ImVec2 viewportPos = ImVec2(0.0f, 0.0f);
    ImVec2 viewportSize = ImVec2(0.0f, 0.0f);
};
