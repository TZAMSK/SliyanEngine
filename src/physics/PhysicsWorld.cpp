#pragma once
#include <glm/glm.hpp>
#include "physics/RigidBody.hpp"
#include "physics/PhysicsWorld.hpp"

#include <algorithm>

void PhysicsWorld::addBody(RigidBody *b)
{
    if (b)
        m_Bodies.push_back(b);
}

void PhysicsWorld::removeBody(RigidBody *b)
{
    if (!b)
        return;

    m_Bodies.erase(std::remove(m_Bodies.begin(), m_Bodies.end(), b), m_Bodies.end());
}

void PhysicsWorld::step(float dt)
{
    for (auto *b : m_Bodies)
    {
        if (b->isStatic())
            continue;

        b->velocity += (gravity + b->force * b->invMass) * dt;
        b->position += b->velocity * dt;
        b->force = glm::vec3(0);
    }
}
