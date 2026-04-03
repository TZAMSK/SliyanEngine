#include "render/gizmo/Gizmo.hpp"

#include "render/gizmo/TranslationGizmoStrategy.hpp"
#include "scene/shapes/Shape.hpp"
#include "ImGuizmo.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Gizmo::Gizmo() : strategy(std::make_unique<TranslationGizmoStrategy>())
{
    strategy->apply(*this);
}

ImGuizmo::OPERATION &Gizmo::getOperation()
{
    return operation;
}

const ImGuizmo::OPERATION &Gizmo::getOperation() const
{
    return operation;
}

void Gizmo::setOperation(ImGuizmo::OPERATION new_operation)
{
    operation = new_operation;
}

void Gizmo::applyStrategy(std::unique_ptr<IGizmoStrategy> newStrategy)
{
    strategy = std::move(newStrategy);
    strategy->apply(*this);
}

const IGizmoStrategy *Gizmo::getStrategy() const
{
    return strategy.get();
}

void Gizmo::applyTransform(Shape &sel) const
{
    switch (operation)
    {
    case ImGuizmo::OPERATION::TRANSLATE:
        sel.setPosition(translation);
        break;
    case ImGuizmo::OPERATION::ROTATE:
        sel.setRotation(rotation);
        break;
    case ImGuizmo::OPERATION::SCALE:
        sel.setScale(scale);
        break;
    default:
        break;
    }
}

void Gizmo::setTranslation(glm::vec3 new_translation)
{
    translation = new_translation;
}

void Gizmo::setRotation(glm::vec3 new_rotation)
{
    rotation = new_rotation;
}

void Gizmo::setScale(glm::vec3 new_scale)
{
    scale = new_scale;
}
