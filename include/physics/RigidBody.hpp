#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct RigidBody
{
    glm::vec3 position{0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 angularVeclocity{0.0f};
    glm::vec3 force{0.0f};

    float invMass = 1.0f;
    float restitution = 0.4f;

    bool isStatic() const
    {
        return invMass == 0.0f;
    }
};
