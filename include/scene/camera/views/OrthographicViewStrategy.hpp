#pragma once

#include "scene/camera/ICameraViewStrategy.hpp"

class OrthographicViewStrategy : public ICameraViewStrategy
{
  public:
    void apply(Camera &camera) const override;
    const char *name() const override;
};
