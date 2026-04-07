#include "scene/shapes/2d/Triangle.hpp"

Triangle::Triangle(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
    : Shape2D(name, position, color)
{
    rebuildMesh();
    uploadToGpu();
}

Triangle::~Triangle()
{
    destroyGpuResources();
}

void Triangle::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(9));

    m_Verts.push_back(0.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(0.0f);
}
