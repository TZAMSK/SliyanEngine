#pragma once

#include "render/gizmo/IGizmoStrategy.hpp"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/glm.hpp>
#include <memory>

class Shape;

class Gizmo
{
  public:
    Gizmo();

    ImGuizmo::OPERATION &getOperation();
    const ImGuizmo::OPERATION &getOperation() const;
    void setOperation(ImGuizmo::OPERATION new_operation);
    void applyStrategy(std::unique_ptr<IGizmoStrategy> newStrategy);
    void applyTransform(Shape &sel) const;
    const IGizmoStrategy *getStrategy() const;

    void setTranslation(glm::vec3 translation);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

  private:
    ImGuizmo::OPERATION operation;
    std::unique_ptr<IGizmoStrategy> strategy;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
};
