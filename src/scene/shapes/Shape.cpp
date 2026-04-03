#include "scene/shapes/Shape.hpp"

static unsigned int s_nextId = 1;

Shape::Shape(const glm::vec3 &position, const glm::vec4 &color) : color(color), position(position), id(s_nextId++)
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

void Shape::setColor(const glm::vec4 &newColor)
{
    color = newColor;
}
