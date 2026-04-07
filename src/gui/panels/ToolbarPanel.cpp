#include "gui/panels/ToolbarPanel.hpp"

#include "app/Application.hpp"
#include "gui/GuiLayout.hpp"
#include "scene/commands/CommandId.hpp"
#include "app/Settings.hpp"

#include "imgui.h"

void drawToolbarPanel(Application &app)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y));
    ImGui::SetNextWindowSize(ImVec2(mainViewport->WorkSize.x, GuiLayout::TOOLBAR_HEIGHT));

    ImGui::Begin("Toolbar", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    if (ImGui::Button("Add Shape"))
    {
        app.executeCommand(CommandId::OpenAddShapeDialog);
    }

    ImGui::SameLine();
    if (ImGui::Button("Iso"))
    {
        app.getScene().getCamera().setPlacementMode(Projection::Perspective);
        app.executeCommand(CommandId::CameraIsometric);
    }

    ImGui::SameLine();
    if (ImGui::Button("Top XY"))
    {
        app.getScene().getCamera().setPlacementMode(Projection::Orthographic);
        app.executeCommand(CommandId::CameraTop);
    }

    ImGui::SameLine();
    if (ImGui::Button("Front XZ"))
    {
        app.getScene().getCamera().setPlacementMode(Projection::Perspective);
        app.executeCommand(CommandId::CameraFront);
    }

    ImGui::SameLine();
    if (ImGui::Button("Side YZ"))
    {
        app.getScene().getCamera().setPlacementMode(Projection::Perspective);
        app.executeCommand(CommandId::CameraSide);
    }

    ImGui::SameLine();
    if (ImGui::Button("Orthographic"))
    {
        app.getScene().getCamera().setPlacementMode(Projection::Orthographic);
        app.executeCommand(CommandId::CameraOrthographic);
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
