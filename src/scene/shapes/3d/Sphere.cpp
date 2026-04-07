#include "scene/shapes/3d/Sphere.hpp"

Sphere::Sphere(const std::string &name, const glm::vec3 &position, const glm::vec4 &color, float radius, int segments)
    : Shape3D(name, position, color), Round(radius, segments)
{
    rebuildMesh();
    uploadToGpu();
}

Sphere::~Sphere()
{
    destroyGpuResources();
}

void Sphere::setRadius(float radius)
{
    Round::setRadius(radius);
    rebuildMesh();
    uploadToGpu();
}

void Sphere::setNbrSegments(int segments)
{
    Round::setNbrSegments(segments);
    rebuildMesh();
    uploadToGpu();
}

void Sphere::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(m_Segments) * static_cast<size_t>(m_Segments) * 6 * 3);

    for (int y = 0; y < m_Segments; y++)
    {
        for (int x = 0; x < m_Segments; x++)
        {
            auto vertAt = [&](int xi, int yi) -> std::array<float, 3> {
                float u = static_cast<float>(xi) / static_cast<float>(m_Segments);
                float v = static_cast<float>(yi) / static_cast<float>(m_Segments);
                float phi = v * M_PI;
                float theta = u * 2.0f * M_PI;
                return {m_Radius * std::sin(phi) * std::cos(theta), m_Radius * std::cos(phi),
                        m_Radius * std::sin(phi) * std::sin(theta)};
            };

            auto tl = vertAt(x, y);
            auto tr = vertAt(x + 1, y);
            auto bl = vertAt(x, y + 1);
            auto br = vertAt(x + 1, y + 1);

            for (float f : tl)
                m_Verts.push_back(f);
            for (float f : bl)
                m_Verts.push_back(f);
            for (float f : tr)
                m_Verts.push_back(f);

            for (float f : tr)
                m_Verts.push_back(f);
            for (float f : bl)
                m_Verts.push_back(f);
            for (float f : br)
                m_Verts.push_back(f);
        }
    }
}
