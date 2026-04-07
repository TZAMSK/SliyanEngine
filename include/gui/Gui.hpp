#pragma once

#include "imgui.h"
#include <string>

enum class PlacementMode
{
    None,
    Triangle,
    Rectangle,
    Circle,
    Cube,
    Sphere
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
    bool isContextMenuDialogOpen() const;
    void openContextMenuDialog();
    void closeContextMenu();

    // Placement mode
    PlacementMode getPlacementMode() const;

    bool isAnyPlacementArmed() const;

    void armTrianglePlacement();
    void armRectanglePlacement();
    void armCirclePlacement();
    void armCubePlacement();
    void armSpherePlacement();
    void disarmPlacement();

    // Console
    void setLog(const std::string &value);
    void appendLog(const std::string &value);
    void clearConsole();
    std::string &logBuffer();

    // Conditional
    bool &demoWindowFlag();
    bool &addShapeDialogFlag();
    bool &contextMenuFlag();

    // Gizmo panel rect
    void setGizmoPanelRect(const ImVec2 &pos, const ImVec2 &size);
    bool isMouseInsideGizmoPanel() const;

    // Custom context panel rect
    void setContextMenuPanelRect(const ImVec2 &pos, const ImVec2 &size);
    bool isMouseInsideContextMenuPanel() const;

  private:
    ImVec2 m_ViewportPos = ImVec2(0.0f, 0.0f);
    ImVec2 m_ViewportSize = ImVec2(0.0f, 0.0f);

    ImVec2 m_GizmoPanelPos = ImVec2(0.0f, 0.0f);
    ImVec2 m_GizmoPanelSize = ImVec2(0.0f, 0.0f);

    ImVec2 m_ContextMenuPanelPos = ImVec2(0.0f, 0.0f);
    ImVec2 m_ContextMenuPanelSize = ImVec2(0.0f, 0.0f);

    bool m_ShowAddShapeDialog = false;
    bool m_ShowContextMenuDialog = false;
    bool m_ShowDemoWindow = false;

    PlacementMode m_PlacementMode = PlacementMode::None;
    std::string m_ConsoleLog;
};
