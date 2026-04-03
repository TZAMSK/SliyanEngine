#pragma once

#include "render/gizmo/IGizmoStrategy.hpp"

class RotationGizmoStrategy : public IGizmoStrategy
{
  public:
    void apply(Gizmo &gizmo) const override;
    const char *name() const override;
};
