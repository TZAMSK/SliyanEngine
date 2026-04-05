#include "scene/camera/views/IsometricViewStrategy.hpp"

#include "scene/camera/Camera.hpp"

#include <glm/glm.hpp>

void IsometricViewStrategy::apply(Camera &camera) const
{
    camera.setView(glm::vec3(5.0f, -8.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

const char *IsometricViewStrategy::name() const
{
    return "Isometric";
}
