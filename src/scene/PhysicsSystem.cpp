#include "scene/shapes/Shape.hpp"
#include "scene/PhysicsSystem.hpp"
#include "physics/ColliderFactory.hpp"
#include "scene/shapes/2d/Circle.hpp"
#include "scene/shapes/3d/Sphere.hpp"

#include <algorithm>

static constexpr float kFixedDt = 1.0f / 240.0f;
static constexpr float kMaxFrameTime = 0.25f;

namespace
{
void applyDragProperties(RigidBody &body, const Shape &shape)
{
    const glm::vec3 s = shape.getScale();

    switch (shape.getType())
    {
    case ShapeType::Triangle:
        body.dragCoefficient = 1.2f;
        body.crossSection = 0.5f * (2.0f * s.x) * (2.0f * s.y);
        break;

    case ShapeType::Rectangle:
        body.dragCoefficient = 1.18f;
        body.crossSection = (4.0f * s.x) * (2.0f * s.y);
        break;

    case ShapeType::Circle: {
        const float r = static_cast<const Circle &>(shape).getRadius() * s.x;
        body.dragCoefficient = 1.17f;
        body.crossSection = glm::pi<float>() * r * r;
        break;
    }

    case ShapeType::Cube:
        body.dragCoefficient = 1.05f;
        body.crossSection = (2.0f * s.x) * (2.0f * s.y);
        break;

    case ShapeType::Sphere: {
        const float r = static_cast<const Sphere &>(shape).getRadius() * s.x;
        body.dragCoefficient = 0.47f;
        body.crossSection = glm::pi<float>() * r * r;
        break;
    }
    }
}
} // namespace

void PhysicsSystem::rebuildCollider(Entry &e)
{
    e.body.parts = makeColliderParts(*e.shape);
    e.body.computeMassProperties(e.mass);
    applyDragProperties(e.body, *e.shape);
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
        e->body.volume = e->shape->getVolume();

        rebuildCollider(*e);
    }

    m_Accumulator = 0.0f;
}

void PhysicsSystem::subStepping(float frameTime)
{
    size_t constexpr steps{2};
    float const sub_dt{frameTime / static_cast<float>(steps)};

    for (size_t i{steps}; i--;)
    {
        m_World.step(sub_dt);
    }
}

void PhysicsSystem::fixedDtAccumulator(float frameTime)
{
    m_Accumulator += std::min(frameTime, kMaxFrameTime);

    while (m_Accumulator >= kFixedDt)
    {
        m_World.step(kFixedDt);
        m_Accumulator -= kFixedDt;
    }
}

void PhysicsSystem::update(float frameTime)
{

    switch (m_TimeStepPattern)
    {
    case TimeStepPattern::SubStepping: {
        subStepping(frameTime);
        break;
    }

    case TimeStepPattern::FixedDt: {
        fixedDtAccumulator(frameTime);
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
