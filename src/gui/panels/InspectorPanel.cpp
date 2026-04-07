#include "gui/panels/InspectorPanel.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "scene/shapes/Round.hpp"

#include "imgui.h"

static const char *shapeTypeName(ShapeType t)
{
    switch (t)
    {
    case ShapeType::Triangle:
        return "Triangle";
    case ShapeType::Rectangle:
        return "Rectangle";
    case ShapeType::Circle:
        return "Circle";
    case ShapeType::Cube:
        return "Cube";
    case ShapeType::Sphere:
        return "Sphere";
    }
    return "Shape";
}

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer, const SelectionManager &selection)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + mainViewport->WorkSize.x - GuiLayout::RIGHT_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + GuiLayout::TOOLBAR_HEIGHT));

    ImGui::SetNextWindowSize(ImVec2(GuiLayout::RIGHT_PANEL_WIDTH,
                                    mainViewport->WorkSize.y - GuiLayout::TOOLBAR_HEIGHT - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Renderer");
    ImGui::Separator();
    ImGui::ColorEdit3("Background", renderer.backgroundColor());

    ImGui::Spacing();
    ImGui::Text("Camera");
    ImGui::Separator();

    const auto &cam = scene.getCamera();
    ImGui::Text("Position: %.2f %.2f %.2f", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
    ImGui::Text("Target:   %.2f %.2f %.2f", cam.getTarget().x, cam.getTarget().y, cam.getTarget().z);

    if (cam.getStrategy())
        ImGui::Text("View mode: %s", cam.getStrategy()->name());

    ImGui::Spacing();
    ImGui::Text("Placement mode: %s", gui.isAnyPlacementArmed() ? "Armed" : "None");

    if (!selection.hasSelection())
    {
        ImGui::End();
        return;
    }

    Shape *selelectedShape = scene.findShapeById(selection.selectedId());
    if (!selelectedShape)
    {
        ImGui::End();
        return;
    }

    ImGui::Spacing();
    ImGui::Text("Selected: %s  (id %u)", shapeTypeName(selelectedShape->getType()), selelectedShape->getId());
    ImGui::Separator();
    ImGui::Spacing();

    static int s_activeTab = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

    ImGui::BeginChild("##InspectorTabs", ImVec2(40.0f, 0.0f), false);

    if (ImGui::Selectable("C", s_activeTab == 0, 0, ImVec2(40.0f, 40.0f)))
        s_activeTab = 0;
    if (ImGui::Selectable("T", s_activeTab == 1, 0, ImVec2(40.0f, 40.0f)))
        s_activeTab = 1;
    if (ImGui::Selectable("P", s_activeTab == 2, 0, ImVec2(40.0f, 40.0f)))
        s_activeTab = 2;
    if (ImGui::Selectable("R", s_activeTab == 3, 0, ImVec2(40.0f, 40.0f)))
        s_activeTab = 3;

    ImGui::EndChild();

    ImGui::PopStyleVar();

    ImGui::SameLine(0.0f, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_Header));

    ImGui::BeginChild("##InspectorContent", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysUseWindowPadding);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 15.0f);

    if (s_activeTab == 0)
        drawColorTab(selelectedShape);
    else if (s_activeTab == 1)
        drawTransformTab(selelectedShape);
    else if (s_activeTab == 2)
        drawPhysicsTab();
    else if (s_activeTab == 3)
        drawRelationsTab(selelectedShape);

    ImGui::EndChild();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::End();
}

void drawColorTab(Shape *shape)
{
    float col[4] = {
        shape->getColor().r,
        shape->getColor().g,
        shape->getColor().b,
        shape->getColor().a,
    };

    ImGui::Text("Color");

    if (ImGui::ColorEdit4("", col))
        shape->setColor(glm::vec4(col[0], col[1], col[2], col[3]));
}

void drawTransformTab(Shape *shape)
{
    ImGui::Text("Position");
    glm::vec3 pos = shape->getPosition();

    ImGui::PushID("Position");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::DragFloat("##X", &pos.x, 0.02f);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &pos.y, 0.02f);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &pos.z, 0.02f);

    ImGui::PopID();

    shape->setPosition(pos);

    ImGui::Spacing();

    ImGui::Text("Rotation");
    glm::vec3 rot = shape->getRotation();

    ImGui::PushID("Rotation");
    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::DragFloat("##X", &rot.x, 0.2f);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &rot.y, 0.2f);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &rot.z, 0.2f);

    ImGui::PopID();

    shape->setRotation(rot);

    ImGui::Spacing();

    ImGui::Text("Scale");
    glm::vec3 scale = shape->getScale();

    ImGui::PushID("Scale");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::DragFloat("##X", &scale.x, 0.01f);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &scale.y, 0.01f);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &scale.z, 0.01f);

    ImGui::PopID();

    shape->setScale(scale);

    if (Round *round = dynamic_cast<Round *>(shape))
    {
        ImGui::Spacing();

        float radius = round->getRadius();
        ImGui::Text("Radius:");

        if (ImGui::InputFloat("##radius", &radius, 1.0f, 1.0f, "%.3f"))
            round->setRadius(radius);

        int segments = round->getNbrSegments();
        ImGui::Text("Segments:");

        if (ImGui::InputInt("##segments", &segments))
            round->setNbrSegments(segments);
    }
}

void drawPhysicsTab()
{
    ImGui::TextDisabled("No physics properties");
}

void drawRelationsTab(Shape *shape)
{
    if (shape->getParent())
    {
        ImGui::Text("Parent: %s", shape->getParent()->getName().c_str());
        bool following = shape->isFollowingParent();

        if (ImGui::Checkbox("Follow parent", &following))
            shape->setFollowingParent(following);
    }
    else
        ImGui::TextDisabled("No parent");

    if (!shape->getChidren().empty())
    {
        for (Shape *child : shape->getChidren())
        {
            ImGui::PushID(child);

            ImGui::Text("%s", child->getName().c_str());
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.0f, 0.0f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.0f, 0.0f, 1.0f));

            const char *buttonLabel = "R";
            float buttonWidth = ImGui::CalcTextSize(buttonLabel).x + ImGui::GetStyle().FramePadding.x * 2.0f;

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonWidth);

            if (ImGui::Button(buttonLabel))
                shape->removeChild(child);

            ImGui::PopStyleColor(3);

            ImGui::PopID();
        }
    }
    else
        ImGui::TextDisabled("No children");
}
