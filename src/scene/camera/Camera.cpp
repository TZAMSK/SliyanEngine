#include "scene/camera/Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &target)
    : position(position), direction(direction), target(target)
{
}

const glm::vec3 &Camera::getPosition() const
{
    return position;
}

const glm::vec3 &Camera::getDirection() const
{
    return direction;
}

const glm::vec3 &Camera::getTarget() const
{
    return target;
}
