#include "gui/Gui.hpp"

#include "app/Application.hpp"
#include "gui/panels/AddShapePopup.hpp"
#include "gui/panels/ConsolePanel.hpp"
#include "gui/panels/InspectorPanel.hpp"
#include "gui/panels/MenuBarPanel.hpp"
#include "gui/panels/ScenePanel.hpp"
#include "gui/panels/ToolbarPanel.hpp"
#include "gui/panels/ViewportPanel.hpp"
#include "gui/panels/GizmoPanel.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Gui::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Gui::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void Gui::draw(Application &app)
{
    drawMenuBarPanel(*this, app);
    drawToolbarPanel(app);
    drawScenePanel(app.getScene());
    drawInspectorPanel(*this, app.getScene(), app.getRenderer(), app.getSelectionManager());
    drawViewportPanel(*this, app.getRenderer());
    drawConsolePanel(*this);

    app.getRenderer().getGizmoRenderer().draw(app, app.getGizmo());
    drawGizmoPanel(*this, app);

    drawAddShapePopup(*this, app);

    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);
}

void Gui::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// Viewport

bool Gui::isMouseInsideViewport() const
{
    const ImVec2 mouse = ImGui::GetMousePos();
    return mouse.x >= viewportPos.x && mouse.y >= viewportPos.y && mouse.x < viewportPos.x + viewportSize.x &&
           mouse.y < viewportPos.y + viewportSize.y;
}

ImVec2 Gui::getViewportPos() const
{
    return viewportPos;
}
ImVec2 Gui::getViewportSize() const
{
    return viewportSize;
}

void Gui::setViewportRect(const ImVec2 &pos, const ImVec2 &size)
{
    viewportPos = pos;
    viewportSize = size;
}

// Add shape

bool Gui::isAddShapeDialogOpen() const
{
    return showAddShapeDialog;
}
void Gui::openAddShapeDialog()
{
    showAddShapeDialog = true;
}
void Gui::closeAddShapeDialog()
{
    showAddShapeDialog = false;
}

// Placement mode

PlacementMode Gui::getPlacementMode() const
{
    return placementMode;
}

bool Gui::isTrianglePlacementArmed() const
{
    return placementMode == PlacementMode::Triangle;
}
bool Gui::isRectanglePlacementArmed() const
{
    return placementMode == PlacementMode::Rectangle;
}
bool Gui::isCubePlacementArmed() const
{
    return placementMode == PlacementMode::Cube;
}
bool Gui::isAnyPlacementArmed() const
{
    return placementMode != PlacementMode::None;
}

void Gui::armTrianglePlacement()
{
    placementMode = PlacementMode::Triangle;
    showAddShapeDialog = false;
}

void Gui::armRectanglePlacement()
{
    placementMode = PlacementMode::Rectangle;
    showAddShapeDialog = false;
}

void Gui::armCubePlacement()
{
    placementMode = PlacementMode::Cube;
    showAddShapeDialog = false;
}

void Gui::disarmPlacement()
{
    placementMode = PlacementMode::None;
    showAddShapeDialog = false;
}

void Gui::disarmTrianglePlacement()
{
    disarmPlacement();
}

// Console

void Gui::setLog(const std::string &value)
{
    consoleLog = value;
}
void Gui::appendLog(const std::string &value)
{
    consoleLog += value;
}
void Gui::clearConsole()
{
    consoleLog.clear();
}
std::string &Gui::logBuffer()
{
    return consoleLog;
}

// Condional

bool &Gui::demoWindowFlag()
{
    return showDemoWindow;
}
bool &Gui::addShapeDialogFlag()
{
    return showAddShapeDialog;
}
