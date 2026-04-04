#pragma once

#include "imgui.h"
#include <string>

enum class PlacementMode
{
    None,
    Triangle,
    Rectangle,
    Cube,
    Circle
};

class Application;
struct GLFWwindow;

class Gui
{
  public:
    void init(GLFWwindow *window);
    void beginFrame();
    void draw(Application &app);
    void endFrame();
    void shutdown();

    // Viewport
    bool isMouseInsideViewport() const;
    ImVec2 getViewportPos() const;
    ImVec2 getViewportSize() const;
    void setViewportRect(const ImVec2 &pos, const ImVec2 &size);

    // Add shape
    bool isAddShapeDialogOpen() const;
    void openAddShapeDialog();
    void closeAddShapeDialog();

    // Placement mode
    PlacementMode getPlacementMode() const;

    bool isTrianglePlacementArmed() const;
    bool isRectanglePlacementArmed() const;
    bool isCubePlacementArmed() const;
    bool isCirclePlacementArmed() const;
    bool isAnyPlacementArmed() const;

    void armTrianglePlacement();
    void armRectanglePlacement();
    void armCirclePlacement();
    void armCubePlacement();
    void disarmPlacement();
    void disarmTrianglePlacement();

    // Console
    void setLog(const std::string &value);
    void appendLog(const std::string &value);
    void clearConsole();
    std::string &logBuffer();

    // Conditional
    bool &demoWindowFlag();
    bool &addShapeDialogFlag();

    // Gizmo panel rect
    void setGizmoPanelRect(const ImVec2 &pos, const ImVec2 &size);
    bool isMouseInsideGizmoPanel() const;

  private:
    ImVec2 viewportPos = ImVec2(0.0f, 0.0f);
    ImVec2 viewportSize = ImVec2(0.0f, 0.0f);

    ImVec2 gizmoPanelPos = ImVec2(0.0f, 0.0f);
    ImVec2 gizmoPanelSize = ImVec2(0.0f, 0.0f);

    bool showAddShapeDialog = false;
    bool showDemoWindow = false;

    PlacementMode placementMode = PlacementMode::None;
    std::string consoleLog;
};
