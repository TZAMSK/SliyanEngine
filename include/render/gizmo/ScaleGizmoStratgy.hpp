#pragma once

#include "render/gizmo/IGizmoStrategy.hpp"

class ScaleGizmoStrategy : public IGizmoStrategy
{
  public:
    void apply(Gizmo &gizmo) const override;
    const char *name() const override;
};
