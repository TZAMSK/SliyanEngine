#pragma once

#include "Shape.hpp"
#include <glm/vec3.hpp>

class Triangle : public Shape
{
  public:
    Triangle(const glm::vec3 &position, const glm::vec4 &color);

    const glm::vec3 &getVertex(int index) const;

  private:
    glm::vec3 vertices[3];
};
