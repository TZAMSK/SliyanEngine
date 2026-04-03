#pragma once

#include "scene/shapes/Shape2D.hpp"

class Rectangle : public Shape2D
{
  public:
    Rectangle(const glm::vec3 &position, const glm::vec4 &color);

    ShapeType getType() const override
    {
        return ShapeType::Rectangle;
    }
};
