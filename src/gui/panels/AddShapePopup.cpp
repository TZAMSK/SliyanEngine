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

    ImGui::Text("Choose a shape to add");
    ImGui::Separator();

    if (ImGui::Button("Triangle"))
    {
        app.executeCommand(CommandId::AddTriangleMode);
    }

    ImGui::SameLine();

    if (ImGui::Button("Rectangle"))
    {
        app.executeCommand(CommandId::AddRectangleMode);
    }

    ImGui::SameLine();

    if (ImGui::Button("Cube"))
    {
        app.executeCommand(CommandId::AddCubeMode);
    }

    ImGui::End();
}
