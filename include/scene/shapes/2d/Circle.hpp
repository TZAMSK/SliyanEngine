#pragma once

#include "scene/shapes/2d/Shape2D.hpp"
#include "scene/shapes/Round.hpp"

class Circle : public Shape2D, public Round
{
  public:
    Circle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, float radius, int segments);
    ~Circle() override;

    ShapeType getType() const override
    {
        return ShapeType::Circle;
    }

    void setRadius(float radius) override;
    void setNbrSegments(int segments) override;

  private:
    void rebuildMesh() override;
};
