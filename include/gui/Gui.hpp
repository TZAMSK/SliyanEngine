#pragma once

#include "gui/PlacementMode.hpp"

#include "imgui.h"

#include <string>

class Application;
class GLFWwindow;

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

    // Add-shape dialog
    bool isAddShapeDialogOpen() const;
    void openAddShapeDialog();
    void closeAddShapeDialog();
    bool &addShapeDialogFlag();

    // Placement mode
    PlacementMode getPlacementMode() const;
    bool isAnyPlacementArmed() const;
    bool isTrianglePlacementArmed() const;
    bool isRectanglePlacementArmed() const;
    bool isCubePlacementArmed() const;

    void armTrianglePlacement();
    void armRectanglePlacement();
    void armCubePlacement();
    void disarmPlacement();
    void disarmTrianglePlacement();

    // Console
    void setLog(const std::string &value);
    void appendLog(const std::string &value);
    void clearConsole();
    std::string &logBuffer();

    bool &demoWindowFlag();

  private:
    ImVec2 viewportPos = {0.0f, 0.0f};
    ImVec2 viewportSize = {0.0f, 0.0f};

    PlacementMode placementMode = PlacementMode::None;
    bool showAddShapeDialog = false;
    bool showDemoWindow = false;

    std::string consoleLog;
};
