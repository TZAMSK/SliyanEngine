#include "scene/shapes/Triangle.hpp"

Triangle::Triangle(const glm::vec3 &position, const glm::vec4 &color) : Shape(position, color)
{
    vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1] = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);
}

const glm::vec3 &Triangle::getVertex(int index) const
{
    return vertices[index];
}
