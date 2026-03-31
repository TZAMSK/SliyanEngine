#pragma once

#include <glm/glm.hpp>

class Shape
{
  public:
    Shape(const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape() = default;

    const glm::vec3 &getPosition() const;
    const glm::vec4 &getColor() const;

  protected:
    glm::vec4 color;
    glm::vec3 position;
};
