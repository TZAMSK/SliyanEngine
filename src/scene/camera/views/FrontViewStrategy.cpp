#include "scene/camera/views/FrontViewStrategy.hpp"

#include "scene/camera/Camera.hpp"

#include <glm/glm.hpp>

void FrontViewStrategy::apply(Camera &camera) const
{
    camera.setView(glm::vec3(0.0f, -12.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

const char *FrontViewStrategy::name() const
{
    return "Front (look down Y)";
}
