#include "scene/shapes/2d/Rectangle.hpp"

Rectangle::Rectangle(const glm::vec3 &position, const glm::vec4 &color) : Shape2D(position, color)
{
    rebuildMesh();
    uploadToGpu();
}

Rectangle::~Rectangle()
{
    destroyGpuResources();
}

void Rectangle::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(18));

    m_Verts.push_back(-2.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(2.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(2.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(-2.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(2.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(0.0f);

    m_Verts.push_back(-2.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(0.0f);
}
