#include "scene/camera/views/OrthographicViewStrategy.hpp"

#include "scene/camera/Camera.hpp"

#include <glm/glm.hpp>

void OrthographicViewStrategy::apply(Camera &camera) const
{
    camera.setView(glm::vec3(50.0f, -80.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

const char *OrthographicViewStrategy::name() const
{
    return "Orthographic";
}
