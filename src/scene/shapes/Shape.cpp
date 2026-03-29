#include "scene/shapes/Shape.hpp"

Shape::Shape(const glm::vec3 &position, const glm::vec4 &color) : color(color), position(position)
{
}

const glm::vec3 &Shape::getPosition() const
{
    return position;
}

const glm::vec4 &Shape::getColor() const
{
    return color;
}
