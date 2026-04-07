#pragma once

#include "scene/shapes/3d/Shape3D.hpp"
#include "scene/shapes/Round.hpp"

class Sphere : public Shape3D, public Round
{
  public:
    Sphere(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, float radius, int segments);
    ~Sphere() override;

    ShapeType getType() const override
    {
        return ShapeType::Sphere;
    }

    void setRadius(float radius);
    void setNbrSegments(int segments);

  private:
    void rebuildMesh() override;
};
