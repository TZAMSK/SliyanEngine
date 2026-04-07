#include "gui/panels/ScenePanel.hpp"

#include "gui/GuiLayout.hpp"
#include "scene/Scene.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "scene/shapes/Shape.hpp"

#include "imgui.h"

#include <cstdio>
#include <iostream>

static bool isDescendant(Shape *ancestor, Shape *candidate)
{
    Shape *current = candidate->getParent();
    while (current)
    {
        if (current == ancestor)
            return true;
        current = current->getParent();
    }
    return false;
}

void drawScenePanel(Scene &scene, SelectionManager &selectionManager)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + GuiLayout::TOOLBAR_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(GuiLayout::LEFT_PANEL_WIDTH,
                                    mainViewport->WorkSize.y - GuiLayout::TOOLBAR_HEIGHT - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Objects (%d)", scene.getShapeCount());
    ImGui::Separator();

    const auto &shapes = scene.getShapes();

    for (std::size_t i = 0; i < shapes.size(); ++i)
    {
        Shape *shape = shapes[i].get();

        if (shape && shape->isRoot())
            drawShapeNode(scene, shape, selectionManager);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SCENE_SHAPE"))
        {
            Shape *draggedShape = *(Shape *const *)payload->Data;

            if (draggedShape && draggedShape->getParent())
                draggedShape->getParent()->removeChild(draggedShape);
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

static void drawShapeNode(Scene &scene, Shape *shape, SelectionManager &selectionManager)
{
    if (!shape)
        return;

    const bool hasChildren = !shape->getChildren().empty();

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;

    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    static Shape *renamingShape = nullptr;
    static char renameBuffer[256] = "";

    ImGui::PushID(shape->getId());

    bool open = false;

    if (renamingShape == shape)
    {
        ImGui::SetNextItemWidth(-1.0f);

        if (ImGui::InputText("##rename", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            shape->setName(renameBuffer);
            renamingShape = nullptr;
        }

        if (ImGui::IsItemDeactivated())
        {
            shape->setName(renameBuffer);
            renamingShape = nullptr;
        }
    }
    else
    {
        open = ImGui::TreeNodeEx("##node", flags, "%s", shape->getName().c_str());

        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseDoubleClicked(0))
            {
                renamingShape = shape;
                std::snprintf(renameBuffer, sizeof(renameBuffer), "%s", shape->getName().c_str());
            }
            else if (ImGui::IsMouseClicked(0))
            {
                std::cout << "clicked" << std::endl;
                selectionManager.setSelected(shape->getId());
            }
        }
    }

    if (ImGui::BeginDragDropSource())
    {
        Shape *payloadShape = shape;
        ImGui::SetDragDropPayload("SCENE_SHAPE", &payloadShape, sizeof(payloadShape));
        ImGui::Text("%s", shape->getName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SCENE_SHAPE"))
        {
            Shape *draggedShape = *(Shape *const *)payload->Data;

            if (draggedShape && draggedShape != shape && !isDescendant(shape, draggedShape))
            {
                if (draggedShape->getParent())
                    draggedShape->getParent()->removeChild(draggedShape);
                shape->addChild(draggedShape);
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (hasChildren && open)
    {
        for (Shape *child : shape->getChildren())
            drawShapeNode(scene, child, selectionManager);

        ImGui::TreePop();
    }

    ImGui::PopID();
}
