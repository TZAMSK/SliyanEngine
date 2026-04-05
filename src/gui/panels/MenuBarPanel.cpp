#include "gui/panels/MenuBarPanel.hpp"

#include "app/Application.hpp"
#include "gui/Gui.hpp"
#include "scene/commands/CommandId.hpp"

#include "imgui.h"

void drawMenuBarPanel(Gui &gui, Application &app)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app.requestClose();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Add Shape"))
            {
                app.executeCommand(CommandId::OpenAddShapeDialog);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Camera"))
        {
            if (ImGui::MenuItem("Isometric"))
            {
                app.getScene().getCamera().setPlacementMode(Projection::Perspective);
                app.executeCommand(CommandId::CameraIsometric);
            }
            if (ImGui::MenuItem("Top (XY)"))
            {
                app.getScene().getCamera().setPlacementMode(Projection::Orthographic);
                app.executeCommand(CommandId::CameraTop);
            }
            if (ImGui::MenuItem("Front (XZ)"))
            {
                app.getScene().getCamera().setPlacementMode(Projection::Perspective);
                app.executeCommand(CommandId::CameraFront);
            }
            if (ImGui::MenuItem("Side (YZ)"))
            {
                app.getScene().getCamera().setPlacementMode(Projection::Perspective);
                app.executeCommand(CommandId::CameraSide);
            }
            if (ImGui::MenuItem("Orthographic"))
            {
                app.getScene().getCamera().setPlacementMode(Projection::Orthographic);
                app.executeCommand(CommandId::CameraOrthographic);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("ImGui Demo", nullptr, &gui.demoWindowFlag());
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
