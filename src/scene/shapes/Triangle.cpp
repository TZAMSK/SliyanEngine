#include "scene/shapes/Triangle.hpp"

Triangle::Triangle(const glm::vec3 &position, const glm::vec4 &color) : Shape2D(position, color)
{
}

const float *Triangle::getVertexData() const
{
    return kVerts;
}

size_t Triangle::getFloatCount() const
{
    return sizeof(kVerts) / sizeof(float);
}
