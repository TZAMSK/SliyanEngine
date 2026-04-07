#include "scene/Scene.hpp"

#include "scene/shapes/2d/Triangle.hpp"
#include "scene/shapes/2d/Rectangle.hpp"
#include "scene/shapes/2d/Circle.hpp"
#include "scene/shapes/3d/Cube.hpp"
#include "scene/shapes/3d/Sphere.hpp"

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

void Scene::addTriangleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Triangle>(name, position, color));
}

void Scene::addRectangleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Rectangle>(name, position, color));
}

void Scene::addCircleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, const float &radius,
                        const int &segments)
{
    shapes.push_back(std::make_unique<Circle>(name, position, color, radius, segments));
}

void Scene::addCubeAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    shapes.push_back(std::make_unique<Cube>(name, position, color));
}

void Scene::addSphereAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, const float &radius,
                        const int &segments)
{
    shapes.push_back(std::make_unique<Sphere>(name, position, color, radius, segments));
}

Shape *Scene::findShapeById(unsigned int id)
{
    for (auto &s : shapes)
    {
        if (s->getId() == id)
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
        if (s->getId() == id)
        {
            return s.get();
        }
    }
    return nullptr;
}
