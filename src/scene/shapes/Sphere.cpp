#include "scene/shapes/Sphere.hpp"

#include <cmath>

Sphere::Sphere(const glm::vec3 &position, const glm::vec4 &color, float radius, int segments)
    : Shape2D(position, color), radius(radius), nbrSegments(segments)
{
    if (nbrSegments < 3)
        nbrSegments = 3;
    rebuildMesh();
    uploadToGpu();
}

Sphere::~Sphere()
{
    destroyGpuResources();
}

void Sphere::rebuildMesh()
{
    kVerts.clear();
    kVerts.reserve(static_cast<size_t>(nbrSegments) * static_cast<size_t>(nbrSegments) * 6 * 3);

    for (int y = 0; y < nbrSegments; y++)
    {
        for (int x = 0; x < nbrSegments; x++)
        {
            auto vertAt = [&](int xi, int yi) -> std::array<float, 3> {
                float u = static_cast<float>(xi) / static_cast<float>(nbrSegments);
                float v = static_cast<float>(yi) / static_cast<float>(nbrSegments);
                float phi = v * M_PI;
                float theta = u * 2.0f * M_PI;
                return {radius * std::sin(phi) * std::cos(theta), radius * std::cos(phi),
                        radius * std::sin(phi) * std::sin(theta)};
            };

            auto tl = vertAt(x, y);
            auto tr = vertAt(x + 1, y);
            auto bl = vertAt(x, y + 1);
            auto br = vertAt(x + 1, y + 1);

            for (float f : tl)
                kVerts.push_back(f);
            for (float f : bl)
                kVerts.push_back(f);
            for (float f : tr)
                kVerts.push_back(f);

            for (float f : tr)
                kVerts.push_back(f);
            for (float f : bl)
                kVerts.push_back(f);
            for (float f : br)
                kVerts.push_back(f);
        }
    }
}

void Sphere::uploadToGpu()
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

void Sphere::destroyGpuResources()
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

const float *Sphere::getVertexData() const
{
    return kVerts.data();
}

size_t Sphere::getFloatCount() const
{
    return kVerts.size();
}

float Sphere::getRadius() const
{
    return radius;
}

void Sphere::setRadius(float newRadius)
{
    if (newRadius < 0.001f)
        newRadius = 0.001f;
    radius = newRadius;
    rebuildMesh();
    uploadToGpu();
}

int Sphere::getNbrSegments() const
{
    return nbrSegments;
}

void Sphere::setNbrSegments(int newNbrSegments)
{
    if (newNbrSegments < 3)
        newNbrSegments = 3;

    nbrSegments = newNbrSegments;
    rebuildMesh();
    uploadToGpu();
}

GLuint Sphere::getVao() const
{
    return vao;
}
