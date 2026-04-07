#pragma once

#include "scene/shapes/Shape.hpp"

class Shape2D : public Shape
{
  public:
    Shape2D(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape2D() = default;
};
