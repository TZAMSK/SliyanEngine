#include "scene/shapes/3d/Cube.hpp"

Cube::Cube(const std::string &name, const glm::vec3 &position, const glm::vec4 &color) : Shape3D(name, position, color)
{
    rebuildMesh();
    uploadToGpu();
}

Cube::~Cube()
{
    destroyGpuResources();
}

void Cube::rebuildMesh()
{
    m_Verts.clear();
    m_Verts.reserve(static_cast<size_t>(108));

    //-Z face
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    //+Z face
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    //-Y face
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    //+Y face
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    //-X face
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    //-X face
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(-1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(-1.0f);
    m_Verts.push_back(1.0f);

    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
    m_Verts.push_back(1.0f);
}
