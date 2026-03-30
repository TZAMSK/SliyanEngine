#pragma once

#include <glm/vec3.hpp>

class Camera
{
  public:
    virtual ~Camera() = default;

    Camera(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &target);

    const glm::vec3 &getPosition() const;
    const glm::vec3 &getDirection() const;
    const glm::vec3 &getTarget() const;
    void setPosition(glm::vec3 position);
    void setDirection(glm::vec3 direction);
    void setTarget(glm::vec3 target);

  private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 target;
};
