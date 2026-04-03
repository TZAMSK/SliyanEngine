#pragma once

#include "scene/shapes/Shape2D.hpp"

#include <glm/vec3.hpp>

class Triangle : public Shape2D
{
  public:
    Triangle(const glm::vec3 &position, const glm::vec4 &color);

    ShapeType getType() const override
    {
        return ShapeType::Triangle;
    }
    const glm::vec3 &getVertex(int index) const;

  private:
    glm::vec3 vertices[3];
};
