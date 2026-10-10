#include "scene/shapes/2d/Circle.hpp"

Circle::Circle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, float radius, int segments)
    : Shape2D(name, position, color), Round(radius, segments)
{
}

Circle::~Circle()
{
    destroyGpuResources();
}

void Circle::setRadius(float radius)
{
    Round::setRadius(radius);
    updateMesh();
}

void Circle::setNbrSegments(int segments)
{
    Round::setNbrSegments(segments);
    updateMesh();
}

void Circle::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(m_Segments) * 3);

    const float pi = 3.14159f;

    for (int i = 0; i < m_Segments; ++i)
    {
        const float angle1 = (static_cast<float>(i) / static_cast<float>(m_Segments)) * 2.0f * pi;
        const float angle2 = (static_cast<float>(i + 1) / static_cast<float>(m_Segments)) * 2.0f * pi;

        m_Verts.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        m_Verts.push_back(glm::vec3(std::cos(angle1) * m_Radius, std::sin(angle1) * m_Radius, 0.0f));
        m_Verts.push_back(glm::vec3(std::cos(angle2) * m_Radius, std::sin(angle2) * m_Radius, 0.0f));
    }
}
