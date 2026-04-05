#pragma once

#include "scene/camera/Camera.hpp"
#include "scene/shapes/Shape.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <vector>

class Scene
{
  public:
    Camera &getCamera();
    const Camera &getCamera() const;

    std::vector<std::unique_ptr<Shape>> &getShapes();
    const std::vector<std::unique_ptr<Shape>> &getShapes() const;

    int getShapeCount() const;

    void addTriangleAt(const glm::vec3 &position, const glm::vec4 &color);
    void addRectangleAt(const glm::vec3 &position, const glm::vec4 &color);
    void addCircleAt(const glm::vec3 &position, const glm::vec4 &color, const float &radius, const int &segments);
    void addCubeAt(const glm::vec3 &position, const glm::vec4 &color);
    void addSphereAt(const glm::vec3 &position, const glm::vec4 &color, const float &radius, const int &segments);

    Shape *findShapeById(unsigned int id);
    const Shape *findShapeById(unsigned int id) const;

  private:
    Camera camera;
    std::vector<std::unique_ptr<Shape>> shapes;
};
