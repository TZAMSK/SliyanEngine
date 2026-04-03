#include "render/gizmo/ScaleGizmoStratgy.hpp"

#include "render/gizmo/Gizmo.hpp"

#include <glm/glm.hpp>

void ScaleGizmoStrategy::apply(Gizmo &gizmo) const
{
    gizmo.setOperation(ImGuizmo::OPERATION::SCALE);
}

const char *ScaleGizmoStrategy::name() const
{
    return "Scale gizmo";
}
