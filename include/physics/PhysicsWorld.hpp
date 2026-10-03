#pragma once
#include <glm/glm.hpp>
#include "physics/RigidBody.hpp"

class PhysicsWorld
{
  public:
    void addBody(RigidBody *b);
    void removeBody(RigidBody *b);
    void step(float t);

    glm::vec3 gravity{0, 0, -9.81f};
    float ground = 0.0f;

  private:
    std::vector<RigidBody *> m_Bodies;
};
