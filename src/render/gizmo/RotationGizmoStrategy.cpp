#include "render/gizmo/RotationGizmoStrategy.hpp"

#include "render/gizmo/Gizmo.hpp"

#include <glm/glm.hpp>

void RotationGizmoStrategy::apply(Gizmo &gizmo) const
{
    gizmo.setOperation(ImGuizmo::OPERATION::ROTATE);
}

const char *RotationGizmoStrategy::name() const
{
    return "Rotation gizmo";
}
