#include "scene/camera/Camera.hpp"

#include "scene/camera/views/IsometricViewStrategy.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : strategy(std::make_unique<IsometricViewStrategy>())
{
    strategy->apply(*this);
}

glm::vec3 &Camera::getPosition()
{
    return position;
}

const glm::vec3 &Camera::getPosition() const
{
    return position;
}

const glm::vec3 &Camera::getTarget() const
{
    return target;
}

const glm::vec3 &Camera::getUp() const
{
    return up;
}

const glm::mat4 Camera::getProjection(ImVec2 viewportSize) const
{
    return glm::perspective(glm::radians(fovDegrees), viewportSize.x / viewportSize.y, 0.1f, 200.0f);
}

const glm::mat4 Camera::getLookAt() const
{
    return glm::lookAt(position, target, up);
}

const float Camera::getFovDegrees() const
{
    return fovDegrees;
}

void Camera::setPosition(glm::vec3 new_position)
{
    position = new_position;
}

void Camera::setView(const glm::vec3 &newPosition, const glm::vec3 &newTarget, const glm::vec3 &newUp)
{
    position = newPosition;
    target = newTarget;
    up = newUp;
}

void Camera::applyStrategy(std::unique_ptr<ICameraViewStrategy> newStrategy)
{
    strategy = std::move(newStrategy);
    strategy->apply(*this);
}

const ICameraViewStrategy *Camera::getStrategy() const
{
    return strategy.get();
}

void Camera::moveCamera(double mouseX, double mouseY)
{
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f * sensitivity), glm::vec3(1, 1, 1));
    glm::vec3 rotated = glm::vec3(R * glm::vec4(position, 1.0f));
    setPosition(rotated);
}
