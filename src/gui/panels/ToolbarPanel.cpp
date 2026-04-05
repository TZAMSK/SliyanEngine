#include "gui/panels/ToolbarPanel.hpp"

#include "app/Application.hpp"
#include "gui/GuiLayout.hpp"
#include "scene/commands/CommandId.hpp"
#include "app/Settings.hpp"

#include "imgui.h"

void drawToolbarPanel(Application &app)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float menuBarHeight = ImGui::GetFrameHeight();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(mainViewport->WorkSize.x, GuiLayout::TOOLBAR_HEIGHT));

    ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Add Shape"))
    {
        app.executeCommand(CommandId::OpenAddShapeDialog);
    }

    ImGui::SameLine();
    if (ImGui::Button("Iso"))
    {
        app.executeCommand(CommandId::CameraIsometric);
    }

    ImGui::SameLine();
    if (ImGui::Button("Top XY"))
    {
        app.executeCommand(CommandId::CameraTop);
    }

    ImGui::SameLine();
    if (ImGui::Button("Front XZ"))
    {
        app.executeCommand(CommandId::CameraFront);
    }

    ImGui::SameLine();
    if (ImGui::Button("Side YZ"))
    {
        app.executeCommand(CommandId::CameraSide);
    }

    ImGui::SameLine();
    if (ImGui::Button("Wireframe"))
    {
        app.getViewportSettings().viewMode = ViewMode::Wireframe;
    }

    ImGui::SameLine();
    if (ImGui::Button("Solid"))
    {
        app.getViewportSettings().viewMode = ViewMode::Solid;
    }

    ImGui::End();
}
