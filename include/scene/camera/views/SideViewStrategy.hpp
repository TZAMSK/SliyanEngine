#pragma once

#include "scene/camera/ICameraViewStrategy.hpp"

class SideViewStrategy : public ICameraViewStrategy
{
  public:
    void apply(Camera &camera) const override;
    const char *name() const override;
};
