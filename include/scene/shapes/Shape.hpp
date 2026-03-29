#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shape
{
  public:
    virtual ~Shape() = default;

    Shape(const glm::vec3 &position, const glm::vec4 &color);

    const glm::vec3 &getPosition() const;
    const glm::vec4 &getColor() const;

  private:
    glm::vec4 color;
    glm::vec3 position;
};
