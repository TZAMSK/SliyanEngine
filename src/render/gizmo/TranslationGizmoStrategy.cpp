#include "render/gizmo/TranslationGizmoStrategy.hpp"

#include "render/gizmo/Gizmo.hpp"

#include <glm/glm.hpp>

void TranslationGizmoStrategy::apply(Gizmo &gizmo) const
{
    gizmo.setOperation(ImGuizmo::OPERATION::TRANSLATE);
}

const char *TranslationGizmoStrategy::name() const
{
    return "Translation gizmo";
}
