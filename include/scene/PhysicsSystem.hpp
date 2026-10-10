#pragma once

#include "shapes/Shape.hpp"
#include "physics/RigidBody.hpp"
#include "physics/PhysicsWorld.hpp"

#include <memory>

enum class TimeStepPattern
{
    FixedDt,
    SubStepping
};

class PhysicsSystem
{
  public:
    void add(Shape *shape, float mass);
    void remove(Shape *shape);
    void update(float frameTime);
    void syncFromShapes();
    TimeStepPattern &getTimeStepPattern();
    void setTimeStepPattern(TimeStepPattern timeStampPattern);

  private:
    struct Entry
    {
        Shape *shape = nullptr;
        float mass = 0.0f;
        RigidBody body;
    };

    static void rebuildCollider(Entry &e);
    void fixedDtAccumulator(float frameTime);
    void subStepping(float frameTime);

    PhysicsWorld m_World;
    std::vector<std::unique_ptr<Entry>> m_Entries;
    float m_Accumulator = 0.0f;
    TimeStepPattern m_TimeStepPattern = TimeStepPattern::SubStepping;
};
