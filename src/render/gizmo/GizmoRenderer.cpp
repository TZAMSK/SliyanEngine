#include "render/gizmo/GizmoRenderer.hpp"
#include "app/Application.hpp"
#include "render/gizmo/Gizmo.hpp"
#include "scene/shapes/Circle.hpp"
#include "scene/shapes/Sphere.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

bool GizmoRenderer::init()
{
    return true;
}

void GizmoRenderer::draw(Application &app, Gizmo &gizmo) const
{
    const ImVec2 gizmoPos = app.getGui().getViewportPos();
    const ImVec2 gizmoSize = app.getGui().getViewportSize();

    if (gizmoSize.x <= 0.0f || gizmoSize.y <= 0.0f)
        return;

    const bool hasSelection = app.getSelectionManager().hasSelection();

    // If mouse is over the gizmo panel, let the panel receive the click.
    const bool mouseOverGizmoPanel = app.getGui().isMouseInsideGizmoPanel();

    const bool gizmoNeedsInput = !mouseOverGizmoPanel && (hasSelection || ImGuizmo::IsOver() || ImGuizmo::IsUsing());

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    if (!gizmoNeedsInput)
        flags |= ImGuiWindowFlags_NoInputs;

    ImGui::SetNextWindowPos(gizmoPos);
    ImGui::SetNextWindowSize(gizmoSize);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("##gizmo_host", nullptr, flags);

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(gizmoPos.x, gizmoPos.y, gizmoSize.x, gizmoSize.y);
    ImGuizmo::SetOrthographic(true);

    if (!app.getSelectionManager().hasSelection())
    {
        ImGui::End();
        return;
    }

    Shape *sel = app.getScene().findShapeById(app.getSelectionManager().selectedId());
    if (sel)
    {
        const glm::mat4 cameraProjection = app.getScene().getCamera().getProjection(gizmoSize);
        const glm::mat4 cameraView = app.getScene().getCamera().getLookAt();

        glm::mat4 transform = sel->getTransform();
        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), gizmo.getOperation(),
                             ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));

        const bool isUsing = ImGuizmo::IsUsing();

        if (isUsing)
        {
            glm::vec3 translation, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation),
                                                  glm::value_ptr(rotation), glm::value_ptr(scale));

            gizmo.setTranslation(translation);
            gizmo.setRotation(rotation);

            if (sel->getType() == ShapeType::Circle)
            {
                Circle *circle = dynamic_cast<Circle *>(sel);

                if (!m_wasUsingLastFrame)
                {
                    m_initialCircleRadius = circle->getRadius();
                }

                float uniformScale = (scale.x + scale.y) / 2.0f;
                float newRadius = m_initialCircleRadius * uniformScale;

                circle->setRadius(newRadius);

                gizmo.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else if (sel->getType() == ShapeType::Sphere)
            {
                Sphere *sphere = dynamic_cast<Sphere *>(sel);

                if (!m_wasUsingLastFrame)
                {
                    m_initialCircleRadius = sphere->getRadius();
                }

                float uniformScale = (scale.x + scale.y) / 2.0f;
                float newRadius = m_initialCircleRadius * uniformScale;

                sphere->setRadius(newRadius);

                gizmo.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else
            {
                gizmo.setScale(scale);
            }

            gizmo.applyTransform(*sel);
        }

        m_wasUsingLastFrame = isUsing;

        if (!isUsing)
        {
            m_initialCircleRadius = 0.0f;
        }
    }

    ImGui::End();
}

void GizmoRenderer::shutdown()
{
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}
