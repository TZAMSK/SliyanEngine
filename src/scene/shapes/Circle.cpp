#include "scene/shapes/Circle.hpp"

#include <cmath>

Circle::Circle(const glm::vec3 &position, const glm::vec4 &color, float radius, int segments)
    : Shape2D(position, color), radius(radius), nbrSegments(segments)
{
    if (nbrSegments < 3)
        nbrSegments = 3;

    rebuildMesh();
    uploadToGpu();
}

Circle::~Circle()
{
    destroyGpuResources();
}

void Circle::rebuildMesh()
{
    kVerts.clear();
    kVerts.reserve(static_cast<size_t>(nbrSegments) * 9);

    const float pi = 3.14159265359f;

    for (int i = 0; i < nbrSegments; ++i)
    {
        const float angle1 = (static_cast<float>(i) / static_cast<float>(nbrSegments)) * 2.0f * pi;
        const float angle2 = (static_cast<float>(i + 1) / static_cast<float>(nbrSegments)) * 2.0f * pi;

        kVerts.push_back(0.0f);
        kVerts.push_back(0.0f);
        kVerts.push_back(0.0f);

        kVerts.push_back(std::cos(angle1) * radius);
        kVerts.push_back(std::sin(angle1) * radius);
        kVerts.push_back(0.0f);

        kVerts.push_back(std::cos(angle2) * radius);
        kVerts.push_back(std::sin(angle2) * radius);
        kVerts.push_back(0.0f);
    }
}

void Circle::uploadToGpu()
{
    if (vao == 0)
        glGenVertexArrays(1, &vao);

    if (vbo == 0)
        glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(kVerts.size() * sizeof(float)), kVerts.data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Circle::destroyGpuResources()
{
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

const float *Circle::getVertexData() const
{
    return kVerts.data();
}

size_t Circle::getFloatCount() const
{
    return kVerts.size();
}

float Circle::getRadius() const
{
    return radius;
}

void Circle::setRadius(float newRadius)
{
    if (newRadius < 0.001f)
        newRadius = 0.001f;

    radius = newRadius;
    rebuildMesh();
    uploadToGpu();
}

int Circle::getNbrSegments() const
{
    return nbrSegments;
}

void Circle::setNbrSegments(int newNbrSegments)
{
    if (newNbrSegments < 3)
        newNbrSegments = 3;

    nbrSegments = newNbrSegments;
    rebuildMesh();
    uploadToGpu();
}

GLuint Circle::getVao() const
{
    return vao;
}
