#pragma once

#include "scene/shapes/ShapeType.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>

#include <glad/glad.h>

#include <string>
#include <vector>

class Shape
{
  public:
    Shape(const std::string &name, const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape() = default;

    // Getter
    const unsigned int &getId() const;

    std::string &getName();
    const std::string &getName() const;

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

    std::vector<Shape *> &getChidren();
    const std::vector<Shape *> &getChildren() const;
    Shape *getParent() const;

    bool isRoot() const;

    // Setter
    void setName(const std::string name);

    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::vec3 &rotation);
    void setScale(const glm::vec3 &scale);

    void setColor(const glm::vec4 &color);

    void addChild(Shape *shape);
    void removeChild(Shape *shape);

    // Render
    virtual void rebuildMesh() = 0;
    void uploadToGpu();
    void destroyGpuResources();

    GLuint getVao() const;

  protected:
    std::string m_Name = "Shape";
    std::vector<Shape *> m_Children;
    Shape *m_Parent = nullptr;

    unsigned int m_Id;
    glm::vec4 m_Color;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Scale = {1.0f, 1.0f, 1.0f};

    std::vector<float> m_Verts;

    GLuint m_Vao = 0;
    GLuint m_Vbo = 0;
};
