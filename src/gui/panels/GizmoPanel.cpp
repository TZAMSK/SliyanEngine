#include "app/Application.hpp"
#include "gui/Gui.hpp"

#include "imgui.h"

void drawGizmoPanel(Gui &gui, Application &app)
{
    ImVec2 viewportPos = app.getGui().getViewportPos();
    ImVec2 panel = ImVec2(viewportPos.x + 10.0f, viewportPos.y + 10.0f);

    ImGui::SetNextWindowPos(panel, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.8f);
    ImGui::SetNextWindowSize(ImVec2(110.0f, 115.0f), ImGuiCond_Always);

    ImGui::Begin("##gizmo_panel", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoFocusOnAppearing);

    if (ImGui::Button("Translation", ImVec2(-1, 0)))
        app.executeCommand(CommandId::GizmoTranslation);
    if (ImGui::Button("Rotation", ImVec2(-1, 0)))
        app.executeCommand(CommandId::GizmoRotation);
    if (ImGui::Button("Scale", ImVec2(-1, 0)))
        app.executeCommand(CommandId::GizmoScale);

    ImGui::End();
}
