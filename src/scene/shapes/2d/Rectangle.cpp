#include "scene/shapes/2d/Rectangle.hpp"

Rectangle::Rectangle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
    : Shape2D(name, position, color)
{
}

Rectangle::~Rectangle()
{
    destroyGpuResources();
}

void Rectangle::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(6));

    m_Verts.push_back(glm::vec3(-2.0f, -1.0f, 0.0f));
    m_Verts.push_back(glm::vec3(2.0f, -1.0f, 0.0f));
    m_Verts.push_back(glm::vec3(2.0f, 1.0f, 0.0f));
    m_Verts.push_back(glm::vec3(-2.0f, -1.0f, 0.0f));
    m_Verts.push_back(glm::vec3(2.0f, 1.0f, 0.0f));
    m_Verts.push_back(glm::vec3(-2.0f, 1.0f, 0.0f));
}
