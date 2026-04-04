#pragma once

#include "scene/shapes/Shape2D.hpp"

#include <glad/glad.h>
#include <vector>

class Circle : public Shape2D
{
  public:
    Circle(const glm::vec3 &position, const glm::vec4 &color, float radius, int segments);
    ~Circle() override;

    ShapeType getType() const override
    {
        return ShapeType::Circle;
    }

    const float *getVertexData() const override;
    size_t getFloatCount() const override;

    size_t getVertexCount() const
    {
        return kVerts.size() / 3;
    }

    float getRadius() const;
    void setRadius(float newRadius);

    int getNbrSegments() const;
    void setNbrSegments(int newNbrSegments);

    GLuint getVao() const;

  private:
    void rebuildMesh();
    void uploadToGpu();
    void destroyGpuResources();

  private:
    std::vector<float> kVerts;

    float radius = 1.0f;
    int nbrSegments = 16;

    GLuint vao = 0;
    GLuint vbo = 0;
};
