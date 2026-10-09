#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "physics/Collider.hpp"

struct RigidBody
{
    glm::vec3 position{0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 angularVelocity{0.0f};
    glm::vec3 force{0.0f};

    float invMass = 1.0f;
    float restitution = 0.4f;
    glm::mat3 invInertiaLocal{0.0f};

    std::vector<ColliderPart> parts;

    bool isStatic() const
    {
        return invMass == 0.0f;
    }

    glm::mat3 invInertiaWorld() const
    {
        glm::mat3 R = glm::mat3_cast(orientation);
        return R * invInertiaLocal * glm::transpose(R);
    }

    void computeMassProperties(float mass)
    {
        invMass = mass > 0.0f ? 1.0f / mass : 0.0f;
        invInertiaLocal = glm::mat3(0.0f);
        if (isStatic() || parts.empty())
            return;

        glm::mat3 I(0.0f);
        for (auto &p : parts)
        {
            glm::mat3 R = glm::mat3_cast(p.localRotation);
            I += R * p.shape->inertia(mass / parts.size()) * glm::transpose(R);
        }
        invInertiaLocal = glm::inverse(I);
    }
};
