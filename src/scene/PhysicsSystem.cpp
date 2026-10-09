#include "scene/shapes/Shape.hpp"
#include "scene/PhysicsSystem.hpp"
#include "physics/ColliderFactory.hpp"

#include <algorithm>

static constexpr float kFixedDt = 1.0f / 240.0f;
static constexpr float kMaxFrameTime = 0.25f;

void PhysicsSystem::rebuildCollider(Entry &e)
{
    e.body.parts = makeColliderParts(*e.shape);
    e.body.computeMassProperties(e.mass);
}

void PhysicsSystem::add(Shape *s, float mass)
{
    if (!s)
        return;

    auto e = std::make_unique<Entry>();
    e->shape = s;
    e->mass = mass;
    e->body.position = s->getPosition();
    e->body.orientation = glm::quat(glm::radians(s->getRotation()));
    rebuildCollider(*e);

    e->shape->setPhysicsAdded(true);
    m_World.addBody(&e->body);
    m_Entries.push_back(std::move(e));
}

void PhysicsSystem::remove(Shape *shape)
{
    auto it = std::find_if(m_Entries.begin(), m_Entries.end(),
                           [shape](const std::unique_ptr<Entry> &e) { return e->shape == shape; });
    if (it == m_Entries.end())
        return;

    m_World.removeBody(&(*it)->body);
    m_Entries.erase(it);
}

void PhysicsSystem::syncFromShapes()
{
    for (auto &e : m_Entries)
    {
        e->body.position = e->shape->getPosition();
        e->body.orientation = glm::quat(glm::radians(e->shape->getRotation()));
        e->body.velocity = glm::vec3(0.0f);
        e->body.angularVelocity = glm::vec3(0.0f);

        rebuildCollider(*e);
    }

    m_Accumulator = 0.0f;
}

void PhysicsSystem::update(float frameTime)
{

    switch (m_TimeStepPattern)
    {
    case TimeStepPattern::SubStepping: {
        size_t constexpr steps{8};
        float const sub_dt{frameTime / static_cast<float>(steps)};

        for (size_t i{steps}; i--;)
        {
            m_World.step(sub_dt);
        }
        break;
    }

    case TimeStepPattern::FixedDt: {
        m_Accumulator += std::min(frameTime, kMaxFrameTime);

        while (m_Accumulator >= kFixedDt)
        {
            m_World.step(kFixedDt);
            m_Accumulator -= kFixedDt;
        }

        break;
    }
    }

    for (auto &e : m_Entries)
    {
        if (e->body.isStatic())
            continue;
        e->shape->setPosition(e->body.position);
        e->shape->setRotation(glm::degrees(glm::eulerAngles(e->body.orientation)));
    }
}

TimeStepPattern &PhysicsSystem::getTimeStepPattern()
{
    return m_TimeStepPattern;
}

void PhysicsSystem::setTimeStepPattern(TimeStepPattern timeStepPattern)
{
    m_TimeStepPattern = timeStepPattern;
}
