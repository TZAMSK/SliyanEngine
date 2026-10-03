#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <memory>
#include <vector>

class Collider
{
  public:
    virtual ~Collider() = default;

    virtual glm::vec3 support(const glm::vec3 &dir) const = 0;
    virtual glm::mat3 inertia(float mass) const = 0;
    virtual float boundingRadius() const = 0;
};

class SphereCollider : public Collider
{
  public:
    explicit SphereCollider(float radius) : m_Radius(radius)
    {
    }

    glm::vec3 support(const glm::vec3 &d) const override
    {
        float len = glm::length(d);
        return len > 1e-8f ? d * (m_Radius / len) : glm::vec3(0.0f);
    }
    glm::mat3 inertia(float m) const override
    {
        return glm::mat3(0.4f * m * m_Radius * m_Radius);
    }
    float boundingRadius() const override
    {
        return m_Radius;
    }

  private:
    float m_Radius;
};

class BoxCollider : public Collider
{
  public:
    explicit BoxCollider(const glm::vec3 &halfExtents) : m_Half(halfExtents)
    {
    }

    glm::vec3 support(const glm::vec3 &d) const override
    {
        return {d.x >= 0 ? m_Half.x : -m_Half.x, d.y >= 0 ? m_Half.y : -m_Half.y, d.z >= 0 ? m_Half.z : -m_Half.z};
    }
    glm::mat3 inertia(float m) const override
    {
        glm::mat3 I(0.0f);
        I[0][0] = m / 3.0f * (m_Half.y * m_Half.y + m_Half.z * m_Half.z);
        I[1][1] = m / 3.0f * (m_Half.x * m_Half.x + m_Half.z * m_Half.z);
        I[2][2] = m / 3.0f * (m_Half.x * m_Half.x + m_Half.y * m_Half.y);
        return I;
    }
    float boundingRadius() const override
    {
        return glm::length(m_Half);
    }

  private:
    glm::vec3 m_Half;
};

class ConvexHullCollider : public Collider
{
  public:
    explicit ConvexHullCollider(std::vector<glm::vec3> points) : m_Points(std::move(points))
    {
        glm::vec3 lo(1e30f), hi(-1e30f);
        for (auto &p : m_Points)
        {
            lo = glm::min(lo, p);
            hi = glm::max(hi, p);
            m_Radius = std::max(m_Radius, glm::length(p));
        }
        m_Half = 0.5f * (hi - lo);
    }

    glm::vec3 support(const glm::vec3 &d) const override
    {
        glm::vec3 best = m_Points.front();
        float bestDot = glm::dot(best, d);
        for (auto &p : m_Points)
        {
            float v = glm::dot(p, d);
            if (v > bestDot)
                bestDot = v, best = p;
        }
        return best;
    }
    glm::mat3 inertia(float m) const override
    {
        return BoxCollider(m_Half).inertia(m);
    }
    float boundingRadius() const override
    {
        return m_Radius;
    }

  private:
    std::vector<glm::vec3> m_Points;
    glm::vec3 m_Half{0.0f};
    float m_Radius = 0.0f;
};

struct ColliderPart
{
    std::shared_ptr<const Collider> shape;
    glm::vec3 localPosition{0.0f};
    glm::quat localRotation{1.0f, 0.0f, 0.0f, 0.0f};
};
