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
    drawScenePanel(app.getScene(), app.getSelectionManager());
    drawInspectorPanel(*this, app.getScene(), app.getRenderer(), app.getSelectionManager());
    drawViewportPanel(*this, app.getRenderer());
    drawConsolePanel(*this);

    {
        ImVec2 viewportPos = getViewportPos();
        ImVec2 panelPos = ImVec2(viewportPos.x + 10.0f, viewportPos.y + 10.0f);
        ImVec2 panelSize = ImVec2(110.0f, 115.0f);
        setGizmoPanelRect(panelPos, panelSize);
    }

    app.getRenderer().getGizmoRenderer().draw(app, app.getGizmo());
    drawGizmoPanel(*this, app);

    drawAddShapePopup(*this, app);

    if (m_ShowDemoWindow)
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
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
    return mouse.x >= m_ViewportPos.x && mouse.y >= m_ViewportPos.y && mouse.x < m_ViewportPos.x + m_ViewportSize.x &&
           mouse.y < m_ViewportPos.y + m_ViewportSize.y;
}

ImVec2 Gui::getViewportPos() const
{
    return m_ViewportPos;
}

ImVec2 Gui::getViewportSize() const
{
    return m_ViewportSize;
}

void Gui::setViewportRect(const ImVec2 &viewportPos, const ImVec2 &viewportSize)
{
    m_ViewportPos = viewportPos;
    m_ViewportSize = viewportSize;
}

// Add shape

bool Gui::isAddShapeDialogOpen() const
{
    return m_ShowAddShapeDialog;
}

void Gui::openAddShapeDialog()
{
    m_ShowAddShapeDialog = true;
}

void Gui::closeAddShapeDialog()
{
    m_ShowAddShapeDialog = false;
}

// Placement mode

PlacementMode Gui::getPlacementMode() const
{
    return m_PlacementMode;
}

bool Gui::isAnyPlacementArmed() const
{
    return m_PlacementMode != PlacementMode::None;
}

void Gui::armTrianglePlacement()
{
    m_PlacementMode = PlacementMode::Triangle;
    m_ShowAddShapeDialog = false;
}

void Gui::armRectanglePlacement()
{
    m_PlacementMode = PlacementMode::Rectangle;
    m_ShowAddShapeDialog = false;
}

void Gui::armCirclePlacement()
{
    m_PlacementMode = PlacementMode::Circle;
    m_ShowAddShapeDialog = false;
}

void Gui::armCubePlacement()
{
    m_PlacementMode = PlacementMode::Cube;
    m_ShowAddShapeDialog = false;
}

void Gui::armSpherePlacement()
{
    m_PlacementMode = PlacementMode::Sphere;
    m_ShowAddShapeDialog = false;
}

void Gui::disarmPlacement()
{
    m_PlacementMode = PlacementMode::None;
    m_ShowAddShapeDialog = false;
}

// Console

void Gui::setLog(const std::string &value)
{
    m_ConsoleLog = value;
}

void Gui::appendLog(const std::string &value)
{
    m_ConsoleLog += value;
}

void Gui::clearConsole()
{
    m_ConsoleLog.clear();
}

std::string &Gui::logBuffer()
{
    return m_ConsoleLog;
}

// Conditional

bool &Gui::demoWindowFlag()
{
    return m_ShowDemoWindow;
}

bool &Gui::addShapeDialogFlag()
{
    return m_ShowAddShapeDialog;
}

// Gizmo panel rect

void Gui::setGizmoPanelRect(const ImVec2 &gizmoPanelPos, const ImVec2 &gizmoPanelSize)
{
    m_GizmoPanelPos = gizmoPanelPos;
    m_GizmoPanelSize = gizmoPanelSize;
}

bool Gui::isMouseInsideGizmoPanel() const
{
    const ImVec2 mouse = ImGui::GetMousePos();

    return mouse.x >= m_GizmoPanelPos.x && mouse.y >= m_GizmoPanelPos.y &&
           mouse.x < m_GizmoPanelPos.x + m_GizmoPanelSize.x && mouse.y < m_GizmoPanelPos.y + m_GizmoPanelSize.y;
}
