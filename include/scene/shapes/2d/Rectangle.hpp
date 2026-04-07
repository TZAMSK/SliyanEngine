#pragma once

#include "scene/shapes/2d/Shape2D.hpp"

class Rectangle : public Shape2D
{
  public:
    Rectangle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    ~Rectangle() override;

    ShapeType getType() const override
    {
        return ShapeType::Rectangle;
    }

  private:
    void rebuildMesh() override;
};
