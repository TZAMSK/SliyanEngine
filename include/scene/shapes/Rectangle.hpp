#pragma once

#include "scene/shapes/Shape2D.hpp"

class Rectangle : public Shape2D
{
  public:
    Rectangle(const glm::vec3 &position, const glm::vec4 &color);

    ShapeType getType() const override
    {
        return ShapeType::Rectangle;
    }

    const float *getVertexData() const override;
    size_t getFloatCount() const override;

  private:
    static constexpr float kVerts[] = {
        -1.0f, -0.75f, 0.0f, 1.0f, -0.75f, 0.0f, 1.0f,  0.75f, 0.0f,
        -1.0f, -0.75f, 0.0f, 1.0f, 0.75f,  0.0f, -1.0f, 0.75f, 0.0f,
    };
};
