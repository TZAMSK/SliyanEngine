#pragma once

#include "scene/shapes/2d/Shape2D.hpp"

class Triangle : public Shape2D
{
  public:
    Triangle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    ~Triangle() override;

    ShapeType getType() const override
    {
        return ShapeType::Triangle;
    }

  private:
    void rebuildMesh() override;
};
