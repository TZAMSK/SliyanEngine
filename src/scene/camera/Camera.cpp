#include "scene/camera/Camera.hpp"

#include "scene/camera/views/IsometricViewStrategy.hpp"

Camera::Camera() : strategy(std::make_unique<IsometricViewStrategy>())
{
    strategy->apply(*this);
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

float Camera::getFovDegrees() const
{
    return fovDegrees;
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
