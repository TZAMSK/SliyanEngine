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

    Shape *sel = scene.findShapeById(selection.selectedId());
    if (!sel)
    {
        ImGui::End();
        return;
    }

    ImGui::Spacing();
    ImGui::Text("Selected: %s  (id %u)", shapeTypeName(sel->getType()), sel->getId());
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

    ImGui::EndChild();

    ImGui::PopStyleVar();

    ImGui::SameLine(0.0f, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_Header));

    ImGui::BeginChild("##InspectorContent", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysUseWindowPadding);

    if (s_activeTab == 0)
        drawColorTab(sel);
    else if (s_activeTab == 1)
        drawTransformTab(sel);
    else if (s_activeTab == 2)
        drawPhysicsTab();

    ImGui::EndChild();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::End();
}

void drawColorTab(Shape *sel)
{
    float col[4] = {
        sel->getColor().r,
        sel->getColor().g,
        sel->getColor().b,
        sel->getColor().a,
    };

    ImGui::Text("Color");

    ImGui::SetNextItemWidth(-FLT_MIN);
    if (ImGui::ColorEdit4("", col))
        sel->setColor(glm::vec4(col[0], col[1], col[2], col[3]));
}

void drawTransformTab(Shape *sel)
{
    ImGui::Text("Position");
    glm::vec3 pos = sel->getPosition();

    ImGui::PushID("Position");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##X", &pos.x, 0.5f);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Y", &pos.y, 0.5f);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Z", &pos.z, 0.5);

    ImGui::PopID();

    sel->setPosition(pos);

    ImGui::Spacing();

    ImGui::Text("Rotation");
    glm::vec3 rot = sel->getRotation();

    ImGui::PushID("Rotation");
    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##X", &rot.x);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Y", &rot.y);

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Z", &rot.z);

    ImGui::PopID();

    sel->setRotation(rot);

    ImGui::Spacing();

    ImGui::Text("Scale");
    glm::vec3 scale = sel->getScale();

    ImGui::PushID("Scale");

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##X", &scale.x);

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragFloat("##Y", &scale.y);

    ImGui::Text("Z");
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &scale.z);

    ImGui::PopID();

    sel->setScale(scale);

    if (Round *round = dynamic_cast<Round *>(sel))
    {
        ImGui::Spacing();

        float radius = round->getRadius();
        ImGui::Text("Radius:");

        ImGui::SetNextItemWidth(-FLT_MIN);
        if (ImGui::InputFloat("##radius", &radius, 1.0f, 1.0f, "%.3f"))
            round->setRadius(radius);

        int segments = round->getNbrSegments();
        ImGui::Text("Segments:");

        ImGui::SetNextItemWidth(-FLT_MIN);
        if (ImGui::InputInt("##segments", &segments))
            round->setNbrSegments(segments);
    }
}

void drawPhysicsTab()
{
    ImGui::TextDisabled("No physics properties");
}
