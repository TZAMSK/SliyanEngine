#pragma once

#include "scene/camera/ICameraViewStrategy.hpp"

#include <glm/glm.hpp>
#include <memory>

class Camera
{
  public:
    Camera();

    const glm::vec3 &getPosition() const;
    const glm::vec3 &getTarget() const;
    const glm::vec3 &getUp() const;
    float getFovDegrees() const;

    void setView(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up);
    void applyStrategy(std::unique_ptr<ICameraViewStrategy> newStrategy);

    const ICameraViewStrategy *getStrategy() const;

  private:
    glm::vec3 position{8.0f, -8.0f, 6.0f};
    glm::vec3 target{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 0.0f, 1.0f};
    float fovDegrees = 45.0f;
    std::unique_ptr<ICameraViewStrategy> strategy;
};
