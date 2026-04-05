#include "gui/panels/InspectorPanel.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "scene/shapes/Circle.hpp"
#include "scene/shapes/Sphere.hpp"

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
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + mainViewport->WorkSize.x - GuiLayout::RIGHT_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(GuiLayout::RIGHT_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Renderer color
    ImGui::Text("Renderer");
    ImGui::Separator();
    ImGui::ColorEdit3("Background", renderer.backgroundColor());

    // Camera information
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

    // Selected shape info
    if (selection.hasSelection())
    {
        Shape *sel = scene.findShapeById(selection.selectedId());
        if (sel)
        {
            // Color
            if (ImGui::CollapsingHeader("Color"))
            {
                ImGui::Text("Color:");
                float col[4] = {
                    sel->getColor().r,
                    sel->getColor().g,
                    sel->getColor().b,
                    sel->getColor().a,
                };
                if (ImGui::ColorEdit4("", col))
                {
                    sel->setColor(glm::vec4(col[0], col[1], col[2], col[3]));
                }
            }

            // Transforfms
            if (ImGui::CollapsingHeader("Transforfms"))
            {
                ImGui::Spacing();
                ImGui::Text("Selected: %s  (id %u)", shapeTypeName(sel->getType()), sel->id);
                ImGui::Separator();

                // Pos
                ImGui::Text("Position: ");
                glm::vec3 pos = sel->getPosition();

                ImGui::PushID("Position");

                ImGui::Text("X");
                ImGui::SameLine();
                ImGui::InputFloat("##X", &pos.x);

                ImGui::Text("Y");
                ImGui::SameLine();
                ImGui::InputFloat("##Y", &pos.y);

                ImGui::Text("Z");
                ImGui::SameLine();
                ImGui::InputFloat("##Z", &pos.z);

                ImGui::PopID();

                sel->setPosition(pos);

                // Rotation
                ImGui::Text("Rotation: ");
                glm::vec3 rot = sel->getRotation();

                ImGui::PushID("Rotation");

                ImGui::Text("X");
                ImGui::SameLine();
                ImGui::InputFloat("##X", &rot.x);

                ImGui::Text("Y");
                ImGui::SameLine();
                ImGui::InputFloat("##Y", &rot.y);

                ImGui::Text("Z");
                ImGui::SameLine();
                ImGui::InputFloat("##Z", &rot.z);

                ImGui::PopID();

                sel->setRotation(rot);

                // Scale
                ImGui::Text("Scale: ");
                glm::vec3 scale = sel->getScale();

                ImGui::PushID("Scale");

                ImGui::Text("X");
                ImGui::SameLine();
                ImGui::InputFloat("##X", &scale.x);

                ImGui::Text("Y");
                ImGui::SameLine();
                ImGui::InputFloat("##Y", &scale.y);

                ImGui::Text("Z");
                ImGui::SameLine();
                ImGui::InputFloat("##Z", &scale.z);

                ImGui::PopID();

                sel->setScale(scale);

                // If Circle
                if (sel->getType() == ShapeType::Circle)
                {
                    Circle *circle = dynamic_cast<Circle *>(sel);
                    float radius = circle->getRadius();

                    ImGui::Text("Radius: ");
                    if (ImGui::InputFloat("##radius", &radius, 1.0f, 1.0f, "%.3f"))
                    {
                        circle->setRadius(radius);
                    }

                    int segments = circle->getNbrSegments();

                    ImGui::Text("Segments: ");
                    if (ImGui::InputInt("##segments", &segments))
                    {
                        circle->setNbrSegments(segments);
                    }
                }

                // If Sphere
                if (sel->getType() == ShapeType::Sphere)
                {
                    Sphere *sphere = dynamic_cast<Sphere *>(sel);
                    float radius = sphere->getRadius();

                    ImGui::Text("Radius: ");
                    if (ImGui::InputFloat("##radius", &radius, 1.0f, 1.0f, "%.3f"))
                    {
                        sphere->setRadius(radius);
                    }

                    int segments = sphere->getNbrSegments();

                    ImGui::Text("Segments: ");
                    if (ImGui::InputInt("##segments", &segments))
                    {
                        sphere->setNbrSegments(segments);
                    }
                }
            }
        }
    }

    ImGui::End();
}
