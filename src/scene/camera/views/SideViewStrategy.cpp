#include "scene/camera/views/SideViewStrategy.hpp"

#include "scene/camera/Camera.hpp"

#include <glm/glm.hpp>

void SideViewStrategy::apply(Camera &camera) const
{
    camera.setView(glm::vec3(12.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

const char *SideViewStrategy::name() const
{
    return "Side (look down X)";
}
