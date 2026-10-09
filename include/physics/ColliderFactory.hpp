#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "physics/Collider.hpp"
#include "scene/shapes/3d/Sphere.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/shapes/ShapeType.hpp"

constexpr float kCubeMeshHalfExtent = 1.0f;
constexpr float kFlatHalfThickness = 0.05f;

inline ColliderPart makePart(std::shared_ptr<const Collider> c)
{
    return ColliderPart{std::move(c)};
}

inline std::vector<glm::vec3> extractVertexPositions(const Shape &s)
{
    const float *data = s.getVertexData();
    const size_t count = s.getVertexCount();
    if (!data || count == 0)
        return {};

    const size_t stride = s.getFloatCount() / count;

    std::vector<glm::vec3> pts;
    pts.reserve(count);
    for (size_t i = 0; i < count; ++i)
        pts.emplace_back(data[i * stride], data[i * stride + 1], data[i * stride + 2]);

    auto less = [](const glm::vec3 &a, const glm::vec3 &b) {
        return a.x != b.x ? a.x < b.x : a.y != b.y ? a.y < b.y : a.z < b.z;
    };
    std::sort(pts.begin(), pts.end(), less);
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    return pts;
}

inline void thickenIfFlat(std::vector<glm::vec3> &pts)
{
    float lo = 1e30f, hi = -1e30f;
    for (auto &p : pts)
    {
        lo = std::min(lo, p.z);
        hi = std::max(hi, p.z);
    }
    if (hi - lo > 1e-4f)
        return;

    std::vector<glm::vec3> out;
    out.reserve(pts.size() * 2);
    for (auto &p : pts)
    {
        out.push_back({p.x, p.y, p.z + kFlatHalfThickness});
        out.push_back({p.x, p.y, p.z - kFlatHalfThickness});
    }
    pts = std::move(out);
}

inline std::vector<ColliderPart> makeColliderParts(const Shape &s)
{
    const glm::vec3 scale = s.getScale();

    switch (s.getType())
    {
        /*
            case ShapeType::Sphere: {
                const float radius = static_cast<const Sphere &>(s).getRadius();
                return {makePart(std::make_shared<SphereCollider>(radius * scale.x))};
            }
        */

    case ShapeType::Cube:
        return {makePart(std::make_shared<BoxCollider>(kCubeMeshHalfExtent * scale))};

    default: {
        auto pts = extractVertexPositions(s);
        if (pts.empty())
            return {};

        for (auto &p : pts)
            p *= scale;
        thickenIfFlat(pts);

        return {makePart(std::make_shared<ConvexHullCollider>(std::move(pts)))};
    }
    }
}
