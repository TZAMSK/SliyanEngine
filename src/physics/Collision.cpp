#include "physics/Collision.hpp"

#include <array>
#include <limits>
#include <utility>
#include <vector>

namespace
{
using glm::vec3;

struct Vertex
{
    vec3 p, a, b;
};

Vertex minkowski(const WorldCollider &A, const WorldCollider &B, const vec3 &dir)
{
    Vertex v;
    v.a = A.support(dir);
    v.b = B.support(-dir);
    v.p = v.a - v.b;
    return v;
}

bool same(const vec3 &a, const vec3 &b)
{
    return glm::dot(a, b) > 0.0f;
}

struct Simplex
{
    std::array<Vertex, 4> v;
    int size = 0;

    void set(std::initializer_list<Vertex> l)
    {
        size = 0;
        for (auto &x : l)
            v[size++] = x;
    }
    void push(const Vertex &x)
    {
        for (int i = std::min(size, 3); i > 0; --i)
            v[i] = v[i - 1];
        v[0] = x;
        size = std::min(size + 1, 4);
    }
};

bool lineCase(Simplex &s, vec3 &dir)
{
    vec3 ab = s.v[1].p - s.v[0].p, ao = -s.v[0].p;
    if (same(ab, ao))
    {
        dir = glm::cross(glm::cross(ab, ao), ab);
        if (glm::dot(dir, dir) < 1e-12f)
            dir =
                glm::abs(ab.x) < 0.9f * glm::length(ab) ? glm::cross(ab, vec3(1, 0, 0)) : glm::cross(ab, vec3(0, 1, 0));
    }
    else
    {
        s.set({s.v[0]});
        dir = ao;
    }
    return false;
}

bool triangleCase(Simplex &s, vec3 &dir)
{
    vec3 a = s.v[0].p, ab = s.v[1].p - a, ac = s.v[2].p - a, ao = -a;
    vec3 abc = glm::cross(ab, ac);

    if (same(glm::cross(abc, ac), ao))
    {
        if (same(ac, ao))
        {
            s.set({s.v[0], s.v[2]});
            dir = glm::cross(glm::cross(ac, ao), ac);
        }
        else
        {
            s.set({s.v[0], s.v[1]});
            return lineCase(s, dir);
        }
    }
    else if (same(glm::cross(ab, abc), ao))
    {
        s.set({s.v[0], s.v[1]});
        return lineCase(s, dir);
    }
    else if (same(abc, ao))
        dir = abc;
    else
    {
        s.set({s.v[0], s.v[2], s.v[1]});
        dir = -abc;
    }
    return false;
}

bool tetraCase(Simplex &s, vec3 &dir)
{
    vec3 a = s.v[0].p, ab = s.v[1].p - a, ac = s.v[2].p - a, ad = s.v[3].p - a, ao = -a;
    vec3 abc = glm::cross(ab, ac), acd = glm::cross(ac, ad), adb = glm::cross(ad, ab);

    if (same(abc, ao))
    {
        s.set({s.v[0], s.v[1], s.v[2]});
        return triangleCase(s, dir);
    }
    if (same(acd, ao))
    {
        s.set({s.v[0], s.v[2], s.v[3]});
        return triangleCase(s, dir);
    }
    if (same(adb, ao))
    {
        s.set({s.v[0], s.v[3], s.v[1]});
        return triangleCase(s, dir);
    }
    return true;
}

bool nextSimplex(Simplex &s, vec3 &dir)
{
    switch (s.size)
    {
    case 2:
        return lineCase(s, dir);
    case 3:
        return triangleCase(s, dir);
    default:
        return tetraCase(s, dir);
    }
}

bool gjk(const WorldCollider &A, const WorldCollider &B, Simplex &s)
{
    vec3 dir = B.position - A.position;
    if (glm::dot(dir, dir) < 1e-12f)
        dir = vec3(1, 0, 0);

    s.push(minkowski(A, B, dir));
    dir = -s.v[0].p;
    if (glm::dot(dir, dir) < 1e-12f)
        dir = vec3(0, 1, 0);

    for (int i = 0; i < 64; ++i)
    {
        Vertex p = minkowski(A, B, dir);
        if (glm::dot(p.p, dir) <= 0.0f)
            return false;
        s.push(p);
        if (nextSimplex(s, dir))
            return true;
    }
    return false;
}

struct Face
{
    int i[3];
    vec3 n;
    float d;
};

Face makeFace(const std::vector<Vertex> &vs, int a, int b, int c)
{
    Face f{{a, b, c}, vec3(0), std::numeric_limits<float>::max()};
    vec3 n = glm::cross(vs[b].p - vs[a].p, vs[c].p - vs[a].p);
    float len = glm::length(n);
    if (len < 1e-10f)
        return f;
    f.n = n / len;
    f.d = glm::dot(f.n, vs[a].p);
    if (f.d < 0.0f)
    {
        f.n = -f.n;
        f.d = -f.d;
        std::swap(f.i[1], f.i[2]);
    }
    return f;
}

Contact makeContact(const std::vector<Vertex> &vs, const Face &f)
{
    const Vertex &v0 = vs[f.i[0]], &v1 = vs[f.i[1]], &v2 = vs[f.i[2]];
    vec3 pt = f.n * f.d;

    vec3 e0 = v1.p - v0.p, e1 = v2.p - v0.p, e2 = pt - v0.p;
    float d00 = glm::dot(e0, e0), d01 = glm::dot(e0, e1), d11 = glm::dot(e1, e1);
    float d20 = glm::dot(e2, e0), d21 = glm::dot(e2, e1);
    float denom = d00 * d11 - d01 * d01;

    float u = 1.0f, v = 0.0f, w = 0.0f;
    if (std::abs(denom) > 1e-12f)
    {
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
    }
    return {u * v0.a + v * v1.a + w * v2.a, f.n, f.d};
}

bool epa(const WorldCollider &A, const WorldCollider &B, const Simplex &s, Contact &out)
{
    std::vector<Vertex> vs(s.v.begin(), s.v.end());
    std::vector<Face> faces = {makeFace(vs, 0, 1, 2), makeFace(vs, 0, 3, 1), makeFace(vs, 0, 2, 3),
                               makeFace(vs, 1, 3, 2)};

    for (int iter = 0; iter < 64; ++iter)
    {
        Face closest =
            *std::min_element(faces.begin(), faces.end(), [](const Face &x, const Face &y) { return x.d < y.d; });

        Vertex sp = minkowski(A, B, closest.n);
        if (glm::dot(closest.n, sp.p) - closest.d < 1e-3f)
        {
            out = makeContact(vs, closest);
            return true;
        }

        std::vector<std::pair<int, int>> edges;
        auto addEdge = [&](int a, int b) {
            auto it = std::find(edges.begin(), edges.end(), std::make_pair(b, a));
            if (it != edges.end())
                edges.erase(it);
            else
                edges.emplace_back(a, b);
        };
        for (size_t k = 0; k < faces.size();)
        {
            const Face &f = faces[k];
            if (glm::dot(f.n, sp.p - vs[f.i[0]].p) > 1e-6f)
            {
                addEdge(f.i[0], f.i[1]);
                addEdge(f.i[1], f.i[2]);
                addEdge(f.i[2], f.i[0]);
                faces[k] = faces.back();
                faces.pop_back();
            }
            else
                ++k;
        }

        int newIndex = (int)vs.size();
        vs.push_back(sp);
        for (auto &e : edges)
            faces.push_back(makeFace(vs, e.first, e.second, newIndex));
    }

    Face best = *std::min_element(faces.begin(), faces.end(), [](const Face &x, const Face &y) { return x.d < y.d; });
    out = makeContact(vs, best);
    return true;
}
} // namespace

bool collide(const WorldCollider &a, const WorldCollider &b, Contact &out)
{
    Simplex s;
    return gjk(a, b, s) && epa(a, b, s, out);
}
