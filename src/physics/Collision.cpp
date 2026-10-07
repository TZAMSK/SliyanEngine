#include "physics/Collision.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

namespace
{
using glm::vec3;

struct Vertex
{
    vec3 minkowskiDifferentPoint;
    vec3 singlePointShapeA;
    vec3 singlePointShapeB;
};

Vertex minkowski(const WorldCollider &colliderA, const WorldCollider &colliderB, const vec3 &searchDirection)
{
    Vertex vertex;
    vertex.singlePointShapeA = colliderA.support(searchDirection);
    vertex.singlePointShapeB = colliderB.support(-searchDirection);
    vertex.minkowskiDifferentPoint = vertex.singlePointShapeA - vertex.singlePointShapeB;
    return vertex;
}

bool isAcute(const vec3 &a, const vec3 &b)
{
    return glm::dot(a, b) > 0.0f;
}

struct Simplex
{
    std::array<Vertex, 4> vertices;
    int size = 0;

    void set(std::initializer_list<Vertex> newVertices)
    {
        size = 0;
        for (const Vertex &vertex : newVertices)
            vertices[size++] = vertex;
    }

    void push(const Vertex &newVertex)
    {
        for (int i = std::min(size, 3); i > 0; --i)
            vertices[i] = vertices[i - 1];
        vertices[0] = newVertex;
        size = std::min(size + 1, 4);
    }
};

bool lineCase(Simplex &simplex, vec3 &searchDirection)
{
    const Vertex newestVertex = simplex.vertices[0];
    const vec3 newest = newestVertex.minkowskiDifferentPoint;
    const vec3 older = simplex.vertices[1].minkowskiDifferentPoint;

    const vec3 edge = older - newest;
    const vec3 toOrigin = -newest;

    if (isAcute(edge, toOrigin))
    {
        // Search perpendicular to the edge, on the side facing the origin
        searchDirection = glm::cross(glm::cross(edge, toOrigin), edge);

        if (glm::dot(searchDirection, searchDirection) < 1e-12f)
        {
            // Origin lies on the edge's line, so any perpendicular direction works
            searchDirection = glm::abs(edge.x) < 0.9f * glm::length(edge) ? glm::cross(edge, vec3(1, 0, 0))
                                                                          : glm::cross(edge, vec3(0, 1, 0));
        }
    }
    else
    {
        // Origin is behind the newest point, so keep only that point
        simplex.set({newestVertex});
        searchDirection = toOrigin;
    }
    return false;
}

bool triangleCase(Simplex &simplex, vec3 &searchDirection)
{
    const Vertex newestVertex = simplex.vertices[0];
    const Vertex vertexB = simplex.vertices[1];
    const Vertex vertexC = simplex.vertices[2];

    const vec3 newest = newestVertex.minkowskiDifferentPoint;
    const vec3 edgeToB = vertexB.minkowskiDifferentPoint - newest;
    const vec3 edgeToC = vertexC.minkowskiDifferentPoint - newest;
    const vec3 toOrigin = -newest;
    const vec3 faceNormal = glm::cross(edgeToB, edgeToC);

    // Is the origin outside the edge newest -> C?
    if (isAcute(glm::cross(faceNormal, edgeToC), toOrigin))
    {
        if (isAcute(edgeToC, toOrigin))
        {
            simplex.set({newestVertex, vertexC});
            searchDirection = glm::cross(glm::cross(edgeToC, toOrigin), edgeToC);
        }
        else
        {
            simplex.set({newestVertex, vertexB});
            return lineCase(simplex, searchDirection);
        }
    }
    // Is the origin outside the edge newest -> B?
    else if (isAcute(glm::cross(edgeToB, faceNormal), toOrigin))
    {
        simplex.set({newestVertex, vertexB});
        return lineCase(simplex, searchDirection);
    }
    // Origin is above the triangle
    else if (isAcute(faceNormal, toOrigin))
    {
        searchDirection = faceNormal;
    }
    // Origin is below the triangle: flip winding so the normal points toward it
    else
    {
        simplex.set({newestVertex, vertexC, vertexB});
        searchDirection = -faceNormal;
    }
    return false;
}

bool tetrahedronCase(Simplex &simplex, vec3 &searchDirection)
{
    const Vertex newestVertex = simplex.vertices[0];
    const Vertex vertexB = simplex.vertices[1];
    const Vertex vertexC = simplex.vertices[2];
    const Vertex vertexD = simplex.vertices[3];

    const vec3 newest = newestVertex.minkowskiDifferentPoint;
    const vec3 edgeToB = vertexB.minkowskiDifferentPoint - newest;
    const vec3 edgeToC = vertexC.minkowskiDifferentPoint - newest;
    const vec3 edgeToD = vertexD.minkowskiDifferentPoint - newest;
    const vec3 toOrigin = -newest;

    const vec3 normalABC = glm::cross(edgeToB, edgeToC);
    const vec3 normalACD = glm::cross(edgeToC, edgeToD);
    const vec3 normalADB = glm::cross(edgeToD, edgeToB);

    if (isAcute(normalABC, toOrigin))
    {
        simplex.set({newestVertex, vertexB, vertexC});
        return triangleCase(simplex, searchDirection);
    }
    if (isAcute(normalACD, toOrigin))
    {
        simplex.set({newestVertex, vertexC, vertexD});
        return triangleCase(simplex, searchDirection);
    }
    if (isAcute(normalADB, toOrigin))
    {
        simplex.set({newestVertex, vertexD, vertexB});
        return triangleCase(simplex, searchDirection);
    }
    // Origin is inside the tetrahedron: the shapes overlap
    return true;
}

bool nextSimplex(Simplex &simplex, vec3 &searchDirection)
{
    switch (simplex.size)
    {
    case 2:
        return lineCase(simplex, searchDirection);
    case 3:
        return triangleCase(simplex, searchDirection);
    default:
        return tetrahedronCase(simplex, searchDirection);
    }
}

bool gjk(const WorldCollider &colliderA, const WorldCollider &colliderB, Simplex &simplex)
{
    vec3 searchDirection = colliderB.position - colliderA.position;
    if (glm::dot(searchDirection, searchDirection) < 1e-12f)
        searchDirection = vec3(1, 0, 0);

    simplex.push(minkowski(colliderA, colliderB, searchDirection));
    searchDirection = -simplex.vertices[0].minkowskiDifferentPoint;
    if (glm::dot(searchDirection, searchDirection) < 1e-12f)
        searchDirection = vec3(0, 1, 0);

    for (int iteration = 0; iteration < 64; ++iteration)
    {
        const Vertex supportVertex = minkowski(colliderA, colliderB, searchDirection);

        // The new point did not pass the origin, so the origin cannot be inside
        if (glm::dot(supportVertex.minkowskiDifferentPoint, searchDirection) <= 0.0f)
            return false;

        simplex.push(supportVertex);
        if (nextSimplex(simplex, searchDirection))
            return true;
    }
    return false;
}

struct Face
{
    int vertexIndices[3];
    vec3 normal;
    float distance; // distance from the origin to the face plane
};

Face makeFace(const std::vector<Vertex> &vertices, int indexA, int indexB, int indexC)
{
    Face face{{indexA, indexB, indexC}, vec3(0), std::numeric_limits<float>::max()};

    const vec3 &pointA = vertices[indexA].minkowskiDifferentPoint;
    const vec3 &pointB = vertices[indexB].minkowskiDifferentPoint;
    const vec3 &pointC = vertices[indexC].minkowskiDifferentPoint;

    const vec3 rawNormal = glm::cross(pointB - pointA, pointC - pointA);
    const float length = glm::length(rawNormal);
    if (length < 1e-10f)
        return face; // degenerate triangle, keep distance at max so it is never picked

    face.normal = rawNormal / length;
    face.distance = glm::dot(face.normal, pointA);
    if (face.distance < 0.0f)
    {
        // Make the normal point away from the origin
        face.normal = -face.normal;
        face.distance = -face.distance;
        std::swap(face.vertexIndices[1], face.vertexIndices[2]);
    }
    return face;
}

Contact makeContact(const std::vector<Vertex> &vertices, const Face &face)
{
    const Vertex &vertex0 = vertices[face.vertexIndices[0]];
    const Vertex &vertex1 = vertices[face.vertexIndices[1]];
    const Vertex &vertex2 = vertices[face.vertexIndices[2]];

    // Point on the face closest to the origin
    const vec3 closestPoint = face.normal * face.distance;

    // Barycentric coordinates of closestPoint inside the triangle
    const vec3 edge0 = vertex1.minkowskiDifferentPoint - vertex0.minkowskiDifferentPoint;
    const vec3 edge1 = vertex2.minkowskiDifferentPoint - vertex0.minkowskiDifferentPoint;
    const vec3 toClosest = closestPoint - vertex0.minkowskiDifferentPoint;

    const float dot00 = glm::dot(edge0, edge0);
    const float dot01 = glm::dot(edge0, edge1);
    const float dot11 = glm::dot(edge1, edge1);
    const float dotClosest0 = glm::dot(toClosest, edge0);
    const float dotClosest1 = glm::dot(toClosest, edge1);
    const float denominator = dot00 * dot11 - dot01 * dot01;

    float weight0 = 1.0f, weight1 = 0.0f, weight2 = 0.0f;
    if (std::abs(denominator) > 1e-12f)
    {
        weight1 = (dot11 * dotClosest0 - dot01 * dotClosest1) / denominator;
        weight2 = (dot00 * dotClosest1 - dot01 * dotClosest0) / denominator;
        weight0 = 1.0f - weight1 - weight2;
    }

    // Apply the same weights to the points on shape A to get the real contact point
    const vec3 contactPoint =
        weight0 * vertex0.singlePointShapeA + weight1 * vertex1.singlePointShapeA + weight2 * vertex2.singlePointShapeA;

    return {contactPoint, face.normal, face.distance};
}

const Face &findClosestFace(const std::vector<Face> &faces)
{
    return *std::min_element(faces.begin(), faces.end(),
                             [](const Face &lhs, const Face &rhs) { return lhs.distance < rhs.distance; });
}

bool epa(const WorldCollider &colliderA, const WorldCollider &colliderB, const Simplex &simplex, Contact &outContact)
{
    std::vector<Vertex> vertices(simplex.vertices.begin(), simplex.vertices.end());
    std::vector<Face> faces = {makeFace(vertices, 0, 1, 2), makeFace(vertices, 0, 3, 1), makeFace(vertices, 0, 2, 3),
                               makeFace(vertices, 1, 3, 2)};

    for (int iteration = 0; iteration < 64; ++iteration)
    {
        const Face closestFace = findClosestFace(faces);

        const Vertex supportVertex = minkowski(colliderA, colliderB, closestFace.normal);

        // Can't expand further: the closest face is on the boundary
        if (glm::dot(closestFace.normal, supportVertex.minkowskiDifferentPoint) - closestFace.distance < 1e-3f)
        {
            outContact = makeContact(vertices, closestFace);
            return true;
        }

        // Remove every face that can see the new point and collect the hole's boundary edges
        std::vector<std::pair<int, int>> horizonEdges;
        auto addEdge = [&](int from, int to) {
            auto reversed = std::find(horizonEdges.begin(), horizonEdges.end(), std::make_pair(to, from));
            if (reversed != horizonEdges.end())
                horizonEdges.erase(reversed); // shared by two removed faces, so it is interior
            else
                horizonEdges.emplace_back(from, to);
        };

        for (size_t faceIndex = 0; faceIndex < faces.size();)
        {
            const Face &face = faces[faceIndex];
            const vec3 &facePoint = vertices[face.vertexIndices[0]].minkowskiDifferentPoint;

            if (glm::dot(face.normal, supportVertex.minkowskiDifferentPoint - facePoint) > 1e-6f)
            {
                addEdge(face.vertexIndices[0], face.vertexIndices[1]);
                addEdge(face.vertexIndices[1], face.vertexIndices[2]);
                addEdge(face.vertexIndices[2], face.vertexIndices[0]);
                faces[faceIndex] = faces.back();
                faces.pop_back();
            }
            else
                ++faceIndex;
        }

        // Patch the hole by connecting the boundary edges to the new vertex
        const int newVertexIndex = (int)vertices.size();
        vertices.push_back(supportVertex);
        for (const auto &edge : horizonEdges)
            faces.push_back(makeFace(vertices, edge.first, edge.second, newVertexIndex));
    }

    // Out of iterations: use the best face found so far
    outContact = makeContact(vertices, findClosestFace(faces));
    return true;
}
} // namespace

bool collide(const WorldCollider &colliderA, const WorldCollider &colliderB, Contact &outContact)
{
    Simplex simplex;
    return gjk(colliderA, colliderB, simplex) && epa(colliderA, colliderB, simplex, outContact);
}
