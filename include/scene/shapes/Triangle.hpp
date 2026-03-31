#pragma once

#include "scene/shapes/Shape.hpp"

#include <array>

class Triangle : public Shape
{
  public:
    Triangle(const glm::vec3 &position, const glm::vec4 &color);

    const glm::vec3 &getVertex(int index) const;

  private:
    std::array<glm::vec3, 3> vertices{};
};
