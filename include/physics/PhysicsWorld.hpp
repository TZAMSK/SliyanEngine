#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "physics/Collision.hpp"
#include "physics/RigidBody.hpp"

class PhysicsWorld
{
  public:
    PhysicsWorld();

    void addBody(RigidBody *b);
    void removeBody(RigidBody *b);
    void step(float dt);

    glm::vec3 gravity{0, 0, -9.81f};
    float ground = 0.0f;

  private:
    struct Manifold
    {
        RigidBody *a;
        RigidBody *b;
        Contact contact;
    };

    void integrateVelocities(float dt);
    void integratePositions(float dt);
    void findBodyContacts(std::vector<Manifold> &out) const;
    void findGroundContacts(std::vector<Manifold> &out);
    void solve(std::vector<Manifold> &contacts);

    static constexpr int kSolverIterations = 8;

    std::vector<RigidBody *> m_Bodies;
    RigidBody m_Ground;
};
