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

const glm::vec3 &Shape::getRotation() const
{
    return rotation;
}

const glm::vec3 &Shape::getScale() const
{
    return scale;
}

void Shape::setPosition(const glm::vec3 &pos)
{
    translation = pos;
}

void Shape::setRotation(const glm::vec3 &rot)
{
    rotation = rot;
}

void Shape::setScale(const glm::vec3 &s)
{
    scale = s;
}

glm::mat4 Shape::getTransform() const
{
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);

    const glm::vec3 rotationRadians = glm::radians(rotation);
    const glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotationRadians));

    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

const glm::vec4 &Shape::getColor() const
{
    return color;
}

void Shape::setColor(const glm::vec4 &newColor)
{
    color = newColor;
}
