#include "scene/camera/views/TopViewStrategy.hpp"

#include "scene/camera/Camera.hpp"

#include <glm/glm.hpp>

void TopViewStrategy::apply(Camera &camera) const
{
    camera.setView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

const char *TopViewStrategy::name() const
{
    return "Top (look down Z)";
}
