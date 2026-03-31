#pragma once

#include "imgui.h"

#include <string>

struct GLFWwindow;
class Scene;
class ViewportRenderer;
class Application;

class Gui
{
  public:
    void init(GLFWwindow *window);
    void beginFrame();
    void draw(Application &app);
    void endFrame();
    void shutdown();

    bool isMouseInsideViewport() const;
    ImVec2 getViewportPos() const;
    ImVec2 getViewportSize() const;

    bool isAddShapeDialogOpen() const;
    void openAddShapeDialog();
    void closeAddShapeDialog();

    bool isTrianglePlacementArmed() const;
    void armTrianglePlacement();
    void disarmTrianglePlacement();

    void setLog(const std::string &value);
    void appendLog(const std::string &value);

    bool &demoWindowFlag();
    bool &addShapeDialogFlag();
    std::string &logBuffer();

    void setViewportRect(const ImVec2 &pos, const ImVec2 &size);

  private:
    bool showDemoWindow = false;
    bool showAddShapeDialog = false;
    bool trianglePlacementArmed = false;
    std::string consoleLog;
    ImVec2 viewportPos{0.0f, 0.0f};
    ImVec2 viewportSize{0.0f, 0.0f};
};
