#pragma once
#include "scene/shapes/ShapeType.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>

class Shape
{
  public:
    Shape(const glm::vec3 &position, const glm::vec4 &color);
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;

    const glm::vec3 &getPosition() const;
    const glm::vec3 &getRotation() const;
    const glm::vec3 &getScale() const;

    void setPosition(const glm::vec3 &pos);
    void setRotation(const glm::vec3 &rot);
    void setScale(const glm::vec3 &s);

    const glm::vec4 &getColor() const;
    void setColor(const glm::vec4 &newColor);

    glm::mat4 getTransform() const;

    unsigned int id;

  protected:
    glm::vec4 color;
    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};
;
