#pragma once

#include "scene/shapes/ShapeType.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>

#include <glad/glad.h>

class Shape
{
  public:
    Shape(const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape() = default;

    // Getter
    const unsigned int &getId() const;

    glm::vec3 &getPosition();
    const glm::vec3 &getPosition() const;
    glm::vec3 &getRotation();
    const glm::vec3 &getRotation() const;
    glm::vec3 &getScale();
    const glm::vec3 &getScale() const;

    glm::mat4 getTransform() const;

    virtual ShapeType getType() const = 0;

    const glm::vec4 &getColor() const;

    const float *getVertexData() const;
    size_t getFloatCount() const;
    size_t getVertexCount() const;

    // Setter
    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::vec3 &rotation);
    void setScale(const glm::vec3 &scale);

    void setColor(const glm::vec4 &color);

    // Render
    virtual void rebuildMesh() = 0;
    void uploadToGpu();
    void destroyGpuResources();

    GLuint getVao() const;

  protected:
    unsigned int m_Id;
    glm::vec4 m_Color;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Scale = {1.0f, 1.0f, 1.0f};

    std::vector<float> m_Verts;

    GLuint m_Vao = 0;
    GLuint m_Vbo = 0;
};
