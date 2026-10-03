#include "physics/PhysicsWorld.hpp"

#include <algorithm>

namespace
{
WorldCollider toWorld(const RigidBody &b, const ColliderPart &p)
{
    return {p.shape.get(), b.position + b.orientation * p.localPosition, b.orientation * p.localRotation};
}

void resolveContact(RigidBody &a, RigidBody &b, const Contact &c)
{
    const glm::vec3 &n = c.normal;
    glm::vec3 ra = c.point - a.position;
    glm::vec3 rb = c.point - b.position;

    glm::vec3 va = a.velocity + glm::cross(a.angularVelocity, ra);
    glm::vec3 vb = b.velocity + glm::cross(b.angularVelocity, rb);
    float vn = glm::dot(vb - va, n);
    if (vn >= 0.0f)
        return;

    float e = std::min(a.restitution, b.restitution);
    if (-vn < 1.0f)
        e = 0.0f;

    glm::mat3 invIa = a.invInertiaWorld(), invIb = b.invInertiaWorld();
    float denom = a.invMass + b.invMass +
                  glm::dot(n, glm::cross(invIa * glm::cross(ra, n), ra) + glm::cross(invIb * glm::cross(rb, n), rb));
    if (denom <= 0.0f)
        return;

    glm::vec3 impulse = (-(1.0f + e) * vn / denom) * n;
    a.velocity -= impulse * a.invMass;
    a.angularVelocity -= invIa * glm::cross(ra, impulse);
    b.velocity += impulse * b.invMass;
    b.angularVelocity += invIb * glm::cross(rb, impulse);
}

void correctPenetration(RigidBody &a, RigidBody &b, const Contact &c)
{
    constexpr float kSlop = 0.005f, kPercent = 0.4f;
    float invSum = a.invMass + b.invMass;
    if (invSum == 0.0f)
        return;
    glm::vec3 corr = c.normal * (std::max(c.depth - kSlop, 0.0f) / invSum * kPercent);
    a.position -= corr * a.invMass;
    b.position += corr * b.invMass;
}
} // namespace

PhysicsWorld::PhysicsWorld()
{
    m_Ground.invMass = 0.0f;
}

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
    integrateVelocities(dt);

    std::vector<Manifold> contacts;
    findBodyContacts(contacts);
    findGroundContacts(contacts);
    solve(contacts);

    integratePositions(dt);
}

void PhysicsWorld::integrateVelocities(float dt)
{
    for (auto *b : m_Bodies)
    {
        if (b->isStatic())
            continue;
        b->velocity += (gravity + b->force * b->invMass) * dt;
        b->force = glm::vec3(0);
    }
}

void PhysicsWorld::integratePositions(float dt)
{
    for (auto *b : m_Bodies)
    {
        if (b->isStatic())
            continue;

        b->velocity *= 0.9995f;
        b->angularVelocity *= 0.998f;

        b->position += b->velocity * dt;

        const glm::vec3 &w = b->angularVelocity;
        glm::quat spin(0.0f, w.x, w.y, w.z);
        b->orientation = glm::normalize(b->orientation + 0.5f * dt * (spin * b->orientation));
    }
}

void PhysicsWorld::findBodyContacts(std::vector<Manifold> &out) const
{
    for (size_t i = 0; i < m_Bodies.size(); ++i)
        for (size_t j = i + 1; j < m_Bodies.size(); ++j)
        {
            RigidBody *A = m_Bodies[i], *B = m_Bodies[j];
            if (A->isStatic() && B->isStatic())
                continue;

            for (auto &pa : A->parts)
                for (auto &pb : B->parts)
                {
                    WorldCollider wa = toWorld(*A, pa), wb = toWorld(*B, pb);

                    float reach = wa.shape->boundingRadius() + wb.shape->boundingRadius();
                    if (glm::distance(wa.position, wb.position) > reach)
                        continue;

                    Contact c;
                    if (collide(wa, wb, c))
                        out.push_back({A, B, c});
                }
        }
}

void PhysicsWorld::findGroundContacts(std::vector<Manifold> &out)
{
    static const glm::vec3 kDirs[] = {{0, 0, -1}, {0.05f, 0, -1}, {-0.05f, 0, -1}, {0, 0.05f, -1}, {0, -0.05f, -1}};

    for (auto *b : m_Bodies)
    {
        if (b->isStatic())
            continue;
        for (auto &part : b->parts)
        {
            WorldCollider w = toWorld(*b, part);
            for (auto &dir : kDirs)
            {
                glm::vec3 p = w.support(glm::normalize(dir));
                if (p.z < ground)
                    out.push_back({b, &m_Ground, {p, glm::vec3(0, 0, -1), ground - p.z}});
            }
        }
    }
}

void PhysicsWorld::solve(std::vector<Manifold> &contacts)
{
    for (auto &m : contacts)
        correctPenetration(*m.a, *m.b, m.contact);

    for (int it = 0; it < kSolverIterations; ++it)
        for (auto &m : contacts)
            resolveContact(*m.a, *m.b, m.contact);
}
