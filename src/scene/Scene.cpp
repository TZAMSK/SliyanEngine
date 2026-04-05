#include "scene/Scene.hpp"

#include "scene/shapes/Triangle.hpp"
#include "scene/shapes/Rectangle.hpp"
#include "scene/shapes/Circle.hpp"
#include "scene/shapes/Cube.hpp"
#include "scene/shapes/Sphere.hpp"

Camera &Scene::getCamera()
{
    return camera;
}

const Camera &Scene::getCamera() const
{
    return camera;
}

std::vector<std::unique_ptr<Shape>> &Scene::getShapes()
{
    return shapes;
}

const std::vector<std::unique_ptr<Shape>> &Scene::getShapes() const
{
    return shapes;
}

int Scene::getShapeCount() const
{
    return static_cast<int>(shapes.size());
}

void Scene::addTriangleAt(const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Triangle>(position, color));
}

void Scene::addRectangleAt(const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Rectangle>(position, color));
}

void Scene::addCircleAt(const glm::vec3 &position, const glm::vec4 &color, const float &radius, const int &segments)
{
    shapes.push_back(std::make_unique<Circle>(position, color, radius, segments));
}

void Scene::addCubeAt(const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Cube>(position, color));
}

void Scene::addSphereAt(const glm::vec3 &position, const glm::vec4 &color, const float &radius, const int &segments)
{
    shapes.push_back(std::make_unique<Sphere>(position, color, radius, segments));
}

Shape *Scene::findShapeById(unsigned int id)
{
    for (auto &s : shapes)
    {
        if (s->id == id)
        {
            return s.get();
        }
    }
    return nullptr;
}

const Shape *Scene::findShapeById(unsigned int id) const
{
    for (const auto &s : shapes)
    {
        if (s->id == id)
        {
            return s.get();
        }
    }
    return nullptr;
}
