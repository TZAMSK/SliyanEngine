#pragma once

#include "scene/shapes/ShapeType.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shape
{
  public:
    Shape(const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape() = default;

    virtual ShapeType getType() const = 0;

    const glm::vec3 &getPosition() const;
    const glm::vec4 &getColor() const;
    void setColor(const glm::vec4 &newColor);

    unsigned int id;

  protected:
    glm::vec4 color;
    glm::vec3 position;
};
