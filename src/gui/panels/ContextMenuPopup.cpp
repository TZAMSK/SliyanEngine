#include "gui/panels/ContextMenuPopup.hpp"

#include "gui/Gui.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "scene/Scene.hpp"
#include "imgui.h"

void drawContextMenuPopup(Gui &gui, SelectionManager &selectionManager, Scene &scene)
{
    if (!gui.isContextMenuDialogOpen())
        return;

    static ImVec2 savedPos = ImVec2(0.0f, 0.0f);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        savedPos = ImGui::GetMousePos();

    ImVec2 panelSize = ImVec2(67.0f, 36.0f);
    gui.setContextMenuPanelRect(savedPos, panelSize);
    ImGui::SetNextWindowPos(savedPos, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.8f);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    bool &open = gui.contextMenuFlag();
    ImGui::Begin("##context_menu", &open,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoFocusOnAppearing);

    if (ImGui::Button("Delete"))
    {
        Shape *shapeToDelete = scene.findShapeById(selectionManager.selectedId());
        if (shapeToDelete)
        {
            gui.appendLog("Deleted " + shapeToDelete->getName());
            shapeToDelete->destroyGpuResources();
            scene.removeShape(shapeToDelete);
        }
        gui.closeContextMenu();
    }

    ImGui::End();
}
