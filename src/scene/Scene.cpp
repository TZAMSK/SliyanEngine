#include "scene/Scene.hpp"

Camera &Scene::getCamera()
{
    return camera;
}

const Camera &Scene::getCamera() const
{
    return camera;
}

std::vector<Triangle> &Scene::getTriangles()
{
    return triangles;
}

const std::vector<Triangle> &Scene::getTriangles() const
{
    return triangles;
}

void Scene::addTriangleAt(const glm::vec3 &position, const glm::vec4 &color)
{
    triangles.emplace_back(position, color);
}
