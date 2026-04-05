#include "gui/panels/AddShapePopup.hpp"

#include "app/Application.hpp"
#include "gui/Gui.hpp"
#include "scene/commands/CommandId.hpp"

#include "imgui.h"

void drawAddShapePopup(Gui &gui, Application &app)
{
    if (!gui.isAddShapeDialogOpen())
        return;

    bool &open = gui.addShapeDialogFlag();

    ImGui::Begin("Add Shape", &open);

    if (ImGui::BeginMenu("2D Shapes"))
    {
        if (ImGui::MenuItem("Triangle"))
        {
            app.executeCommand(CommandId::AddTriangleMode);
        }

        if (ImGui::MenuItem("Rectangle"))
        {
            app.executeCommand(CommandId::AddRectangleMode);
        }

        if (ImGui::MenuItem("Circle"))
        {
            app.executeCommand(CommandId::AddCircleMode);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("3D Shapes"))
    {
        if (ImGui::MenuItem("Cube"))
        {
            app.executeCommand(CommandId::AddCubeMode);
        }

        if (ImGui::MenuItem("Sphere"))
        {
            app.executeCommand(CommandId::AddSphereMode);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Lights"))
    {
        if (ImGui::MenuItem("Cube"))
        {
            app.executeCommand(CommandId::AddCubeMode);
        }

        if (ImGui::MenuItem("Sphere"))
        {
            app.executeCommand(CommandId::AddSphereMode);
        }

        ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Empty Axe"))
    {
        app.executeCommand(CommandId::AddSphereMode);
    }

    ImGui::End();
}
