#include "scene/shapes/Cube.hpp"

constexpr float Cube::kVerts[];

Cube::Cube(const glm::vec3 &position, const glm::vec4 &color) : Shape3D(position, color)
{
}

const float *Cube::getVertexData() const
{
    return kVerts;
}

size_t Cube::getFloatCount() const
{
    return sizeof(kVerts) / sizeof(float);
}
