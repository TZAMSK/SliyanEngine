#pragma once

#include "scene/camera/Camera.hpp"
#include "scene/shapes/Triangle.hpp"

#include <vector>

class Scene
{
  public:
    Camera &getCamera();
    const Camera &getCamera() const;

    std::vector<Triangle> &getTriangles();
    const std::vector<Triangle> &getTriangles() const;

    void addTriangleAt(const glm::vec3 &position, const glm::vec4 &color);

  private:
    Camera camera;
    std::vector<Triangle> triangles;
};
