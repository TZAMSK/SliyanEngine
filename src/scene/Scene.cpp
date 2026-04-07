#include "scene/Scene.hpp"

#include "scene/shapes/2d/Triangle.hpp"
#include "scene/shapes/2d/Rectangle.hpp"
#include "scene/shapes/2d/Circle.hpp"
#include "scene/shapes/3d/Cube.hpp"
#include "scene/shapes/3d/Sphere.hpp"

#include <algorithm>

Camera &Scene::getCamera()
{
    return m_Camera;
}

const Camera &Scene::getCamera() const
{
    return m_Camera;
}

std::vector<std::unique_ptr<Shape>> &Scene::getShapes()
{
    return m_Shapes;
}

const std::vector<std::unique_ptr<Shape>> &Scene::getShapes() const
{
    return m_Shapes;
}

int Scene::getShapeCount() const
{
    return static_cast<int>(m_Shapes.size());
}

void Scene::addTriangleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    m_Shapes.push_back(std::make_unique<Triangle>(name, position, color));
}

void Scene::addRectangleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    m_Shapes.push_back(std::make_unique<Rectangle>(name, position, color));
}

void Scene::addCircleAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, const float &radius,
                        const int &segments)
{
    m_Shapes.push_back(std::make_unique<Circle>(name, position, color, radius, segments));
}

void Scene::addCubeAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
{
    m_Shapes.push_back(std::make_unique<Cube>(name, position, color));
}

void Scene::addSphereAt(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, const float &radius,
                        const int &segments)
{
    m_Shapes.push_back(std::make_unique<Sphere>(name, position, color, radius, segments));
}

void Scene::removeShape(Shape *shape)
{
    if (!shape)
        return;

    auto it = std::find_if(m_Shapes.begin(), m_Shapes.end(),
                           [shape](const std::unique_ptr<Shape> &s) { return s.get() == shape; });

    if (it != m_Shapes.end())
        m_Shapes.erase(it);
}

Shape *Scene::findShapeById(unsigned int id)
{
    for (auto &s : m_Shapes)
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
    for (const auto &s : m_Shapes)
    {
        if (s->getId() == id)
        {
            return s.get();
        }
    }
    return nullptr;
}
