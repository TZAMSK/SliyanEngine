#include "gui/Gui.hpp"

#include "app/Application.hpp"
#include "gui/panels/AddShapePopup.hpp"
#include "gui/panels/ConsolePanel.hpp"
#include "gui/panels/InspectorPanel.hpp"
#include "gui/panels/MenuBarPanel.hpp"
#include "gui/panels/ScenePanel.hpp"
#include "gui/panels/ToolbarPanel.hpp"
#include "gui/panels/ViewportPanel.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
}

void Gui::draw(Application &app)
{
    drawMenuBarPanel(*this, app);
    drawToolbarPanel(app);
    drawScenePanel(app.getScene());
    drawInspectorPanel(*this, app.getScene(), app.getRenderer());
    drawViewportPanel(*this, app.getRenderer());
    drawConsolePanel(*this);
    drawAddShapePopup(*this, app);

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
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

bool Gui::isTrianglePlacementArmed() const
{
    return trianglePlacementArmed;
}

void Gui::armTrianglePlacement()
{
    trianglePlacementArmed = true;
    showAddShapeDialog = false;
}

void Gui::disarmTrianglePlacement()
{
    trianglePlacementArmed = false;
    showAddShapeDialog = false;
}

void Gui::setLog(const std::string &value)
{
    consoleLog = value;
}

void Gui::appendLog(const std::string &value)
{
    consoleLog += value;
}

bool &Gui::demoWindowFlag()
{
    return showDemoWindow;
}

bool &Gui::addShapeDialogFlag()
{
    return showAddShapeDialog;
}

std::string &Gui::logBuffer()
{
    return consoleLog;
}
