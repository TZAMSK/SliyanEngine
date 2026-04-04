#pragma once

#include "scene/shapes/Shape2D.hpp"

#include <glm/vec3.hpp>

class Triangle : public Shape2D
{
  public:
    Triangle(const glm::vec3 &position, const glm::vec4 &color);

    ShapeType getType() const override
    {
        return ShapeType::Triangle;
    }
    const float *getVertexData() const override;
    size_t getFloatCount() const override;

  private:
    static constexpr float kVerts[] = {
        0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    };
};
