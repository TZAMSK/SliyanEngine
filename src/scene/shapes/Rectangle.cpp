#include "scene/shapes/Rectangle.hpp"

constexpr float Rectangle::kVerts[];

Rectangle::Rectangle(const glm::vec3 &position, const glm::vec4 &color) : Shape2D(position, color)
{
}

const float *Rectangle::getVertexData() const
{
    return kVerts;
}

size_t Rectangle::getFloatCount() const
{
    return sizeof(kVerts) / sizeof(float);
}
