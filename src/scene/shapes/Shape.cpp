#include "scene/shapes/Shape.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <algorithm>

static unsigned int s_nextId = 1;

Shape::Shape(const std::string &name, const glm::vec3 &position, const glm::vec4 &color)
    : m_Name(name + " " + std::to_string(s_nextId)), m_Position(position), m_Color(color), m_Id(s_nextId++)
{
}

const unsigned int &Shape::getId() const
{
    return m_Id;
}

std::string &Shape::getName()
{
    return m_Name;
}

const std::string &Shape::getName() const
{
    return m_Name;
}

glm::vec3 &Shape::getPosition()
{
    return m_Position;
}

const glm::vec3 &Shape::getPosition() const
{
    return m_Position;
}

glm::vec3 &Shape::getRotation()
{
    return m_Rotation;
}

const glm::vec3 &Shape::getRotation() const
{
    return m_Rotation;
}

glm::vec3 &Shape::getScale()
{
    return m_Scale;
}

const glm::vec3 &Shape::getScale() const
{
    return m_Scale;
}

glm::mat4 Shape::getTransform() const
{
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);

    const glm::vec3 rotationRadians = glm::radians(m_Rotation);
    const glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotationRadians));

    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

const glm::vec4 &Shape::getColor() const
{
    return m_Color;
}

const float *Shape::getVertexData() const
{
    return m_Verts.data();
}

size_t Shape::getFloatCount() const
{
    return m_Verts.size();
}

size_t Shape::getVertexCount() const
{
    return getFloatCount() / 3;
}

std::vector<Shape *> &Shape::getChidren()
{
    return m_Children;
}
const std::vector<Shape *> &Shape::getChildren() const
{
    return m_Children;
}

Shape *Shape::getParent() const
{
    return m_Parent;
}
bool Shape::isRoot() const
{
    return m_Parent == nullptr;
}

void Shape::setName(std::string name)
{
    m_Name = name;
}

void Shape::setPosition(const glm::vec3 &position)
{
    m_Position = position;
}

void Shape::setRotation(const glm::vec3 &rotation)
{
    m_Rotation = rotation;
}

void Shape::setScale(const glm::vec3 &scale)
{
    m_Scale = scale;
}

void Shape::setColor(const glm::vec4 &color)
{
    m_Color = color;
}

void Shape::addChild(Shape *shape)
{
    if (!shape || shape->m_Parent == this)
        return;

    if (shape->m_Parent)
        shape->m_Parent->removeChild(shape);

    shape->m_Parent = this;
    m_Children.push_back(shape);
}

void Shape::removeChild(Shape *shape)
{
    if (!shape)
        return;

    auto it = std::find(m_Children.begin(), m_Children.end(), shape);
    if (it != m_Children.end())
    {
        (*it)->m_Parent = nullptr;
        m_Children.erase(it);
    }
}

void Shape::uploadToGpu()
{
    if (m_Vao == 0)
        glGenVertexArrays(1, &m_Vao);

    if (m_Vbo == 0)
        glGenBuffers(1, &m_Vbo);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Verts.size() * sizeof(float)), m_Verts.data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Shape::destroyGpuResources()
{
    if (m_Vbo)
    {
        glDeleteBuffers(1, &m_Vbo);
        m_Vbo = 0;
    }

    if (m_Vao)
    {
        glDeleteVertexArrays(1, &m_Vao);
        m_Vao = 0;
    }
}

GLuint Shape::getVao() const
{
    return m_Vao;
}
