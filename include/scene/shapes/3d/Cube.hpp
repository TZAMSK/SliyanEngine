#pragma once

#include "scene/shapes/3d/Shape3D.hpp"

class Cube : public Shape3D
{
  public:
    Cube(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    ~Cube() override;

    ShapeType getType() const override
    {
        return ShapeType::Cube;
    }

  private:
    void rebuildMesh() override;
};
