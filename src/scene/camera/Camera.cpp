#include "scene/camera/Camera.hpp"

#include "scene/camera/views/IsometricViewStrategy.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera() : strategy(std::make_unique<IsometricViewStrategy>())
{
    strategy->apply(*this);
    recalculateAnglesFromPosition();
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

const Projection &Camera::getPlacementMode() const
{
    return projection;
}

void Camera::setPlacementMode(Projection new_projection)
{
    projection = new_projection;
}

const glm::mat4 Camera::getProjection(ImVec2 viewportSize) const
{
    const float aspect = viewportSize.x / viewportSize.y;

    if (projection == Projection::Perspective)
    {
        return glm::perspective(glm::radians(fovDegrees), aspect, 0.1f, 200.0f);
    }
    else if (projection == Projection::Orthographic)
    {
        return glm::ortho(-aspect * orthoScale, aspect * orthoScale, -orthoScale, orthoScale, 0.0f, 300.0f);
    }
    else
    {

        return glm::mat4(1.0f);
    }
}

const glm::mat4 Camera::getLookAt() const
{
    return glm::lookAt(position, target, up);
}

const float Camera::getFovDegrees() const
{
    return fovDegrees;
}

void Camera::setOrthoScale(float new_orthoScale)
{
    orthoScale = new_orthoScale;
}

void Camera::setPosition(glm::vec3 new_position)
{
    position = new_position;
    recalculateAnglesFromPosition();
}

void Camera::setView(const glm::vec3 &newPosition, const glm::vec3 &newTarget, const glm::vec3 &newUp)
{
    position = newPosition;
    target = newTarget;
    up = newUp;
    recalculateAnglesFromPosition();
}

void Camera::applyStrategy(std::unique_ptr<ICameraViewStrategy> newStrategy)
{
    strategy = std::move(newStrategy);
    strategy->apply(*this);
    firstMouse = true;
}

const ICameraViewStrategy *Camera::getStrategy() const
{
    return strategy.get();
}

void Camera::recalculateAnglesFromPosition()
{
    const glm::vec3 offset = position - target;
    const float radius = glm::length(offset);
    pitch = glm::degrees(std::asin(offset.z / radius));
    yaw = glm::degrees(std::atan2(offset.y, offset.x));
    firstMouse = true;
}

void Camera::moveCamera(double mouseX, double mouseY)
{
    if (firstMouse)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
        return;
    }

    const double deltaX = (mouseX - lastMouseX);
    const double deltaY = -(mouseY - lastMouseY);
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    yaw -= deltaX * sensitivity;
    pitch -= deltaY * sensitivity;
    pitch = std::clamp(pitch, -89.0, 89.0);

    const float radius = glm::length(position - target);
    const float yawRad = glm::radians(static_cast<float>(yaw));
    const float pitchRad = glm::radians(static_cast<float>(pitch));

    position.x = target.x + radius * std::cos(pitchRad) * std::cos(yawRad);
    position.y = target.y + radius * std::cos(pitchRad) * std::sin(yawRad);
    position.z = target.z + radius * std::sin(pitchRad);
}

void Camera::zoomCamera(double scrollYOffset)
{
    /*
    const glm::vec3 offset = position - target;
    const float radius = glm::length(offset);

    if (radius < 1.0f)
    {
        lastposition = position;
        return;
    }
    */

    const glm::vec3 dir = glm::normalize(target - position);

    if (projection == Projection::Orthographic)
    {
        setOrthoScale(orthoScale - static_cast<float>(scrollYOffset));
    }
    setPosition(position + dir * static_cast<float>(scrollYOffset));
}
