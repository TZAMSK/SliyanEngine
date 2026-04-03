#pragma once

class Gizmo;

class IGizmoStrategy
{
  public:
    virtual ~IGizmoStrategy() = default;
    virtual void apply(Gizmo &gizmo) const = 0;
    virtual const char *name() const = 0;
};
