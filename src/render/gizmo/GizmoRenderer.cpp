#include "render/gizmo/GizmoRenderer.hpp"
#include "app/Application.hpp"
#include "render/gizmo/Gizmo.hpp"
#include "gui/GuiLayout.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

enum class Transforms
{
    Translation,
    Rotation,
    Scale
};

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

    ImGui::SetNextWindowPos(gizmoPos);
    ImGui::SetNextWindowSize(gizmoSize);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("##gizmo_host", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(gizmoPos.x, gizmoPos.y, gizmoSize.x, gizmoSize.y);
    ImGuizmo::SetOrthographic(false);

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

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation),
                                                  glm::value_ptr(rotation), glm::value_ptr(scale));
            gizmo.setTranslation(translation);
            gizmo.setRotation(rotation);
            gizmo.setScale(scale);

            gizmo.applyTransform(*sel);
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
