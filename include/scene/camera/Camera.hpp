#pragma once

#include "scene/camera/ICameraViewStrategy.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <memory>

class Gui;

enum class Projection
{
    Perspective,
    Orthographic,
};

class Camera
{
  public:
    Camera();

    glm::vec3 &getPosition();
    const glm::vec3 &getPosition() const;
    const glm::vec3 &getTarget() const;
    const glm::vec3 &getUp() const;

    const Projection &getPlacementMode() const;
    void setPlacementMode(Projection projection);

    const glm::mat4 getProjection(ImVec2 viewportSize) const;
    const glm::mat4 getLookAt() const;
    const float getFovDegrees() const;
    void moveCamera(double mouseX, double mouseY);
    void zoomCamera(double srollYOffset);

    void setOrthoScale(float new_orthoScale);
    void setPosition(glm::vec3 new_position);

    void setView(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up);
    void applyStrategy(std::unique_ptr<ICameraViewStrategy> newStrategy);

    void resetFirstMouse()
    {
        firstMouse = true;
    }

    const ICameraViewStrategy *getStrategy() const;

  private:
    void recalculateAnglesFromPosition();

  private:
    double yaw = 0.0;
    double pitch = 0.0;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;

    glm::vec3 lastposition{8.0f, -8.0f, 6.0f};

    float sensitivity = 0.3f;
    glm::vec3 position{8.0f, -8.0f, 6.0f};
    glm::vec3 target{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 0.0f, 1.0f};
    float orthoScale = 12.0f;
    float fovDegrees = 45.0f;
    std::unique_ptr<ICameraViewStrategy> strategy;

    Projection projection = Projection::Perspective;
};
