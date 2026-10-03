#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "physics/Collider.hpp"

struct WorldCollider
{
    const Collider *shape;
    glm::vec3 position;
    glm::quat rotation;

    glm::vec3 support(const glm::vec3 &worldDir) const
    {
        return position + rotation * shape->support(glm::inverse(rotation) * worldDir);
    }
};

struct Contact
{
    glm::vec3 point;
    glm::vec3 normal;
    float depth;
};

bool collide(const WorldCollider &a, const WorldCollider &b, Contact &out);
