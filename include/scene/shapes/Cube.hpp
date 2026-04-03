#pragma once

#include "scene/shapes/Shape3D.hpp"

class Cube : public Shape3D
{
  public:
    Cube(const glm::vec3 &position, const glm::vec4 &color);

    ShapeType getType() const override
    {
        return ShapeType::Cube;
    }
};
