#pragma once

#include "scene/shapes/Shape.hpp"

class Shape3D : public Shape
{
  public:
    Shape3D(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape3D() = default;
};
