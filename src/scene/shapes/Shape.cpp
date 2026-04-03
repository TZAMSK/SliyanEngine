#include "scene/shapes/Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

static unsigned int s_nextId = 1;

Shape::Shape(const glm::vec3 &position, const glm::vec4 &color) : color(color), translation(position), id(s_nextId++)
{
}

const glm::vec3 &Shape::getPosition() const
{
    return translation;
}

void Shape::setPosition(const glm::vec3 &pos)
{
    translation = pos;
}

glm::mat4 Shape::getTransform() const
{
    const glm::mat4 rot = glm::toMat4(glm::quat(rotation));
    return glm::translate(glm::mat4(1.0f), translation) * rot * glm::scale(glm::mat4(1.0f), scale);
}

const glm::vec4 &Shape::getColor() const
{
    return color;
}

void Shape::setColor(const glm::vec4 &newColor)
{
    color = newColor;
}
