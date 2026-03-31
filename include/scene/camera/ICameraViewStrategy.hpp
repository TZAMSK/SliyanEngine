#pragma once

class Camera;

class ICameraViewStrategy
{
  public:
    virtual ~ICameraViewStrategy() = default;
    virtual void apply(Camera &camera) const = 0;
    virtual const char *name() const = 0;
};
