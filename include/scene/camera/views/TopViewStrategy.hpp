#pragma once

#include "scene/camera/ICameraViewStrategy.hpp"

class TopViewStrategy : public ICameraViewStrategy
{
  public:
    void apply(Camera &camera) const override;
    const char *name() const override;
};
