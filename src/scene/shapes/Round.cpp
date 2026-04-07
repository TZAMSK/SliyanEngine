#include "scene/shapes/Round.hpp"

namespace
{
float limitRadius(float radius)
{
    if (radius < 0.001f)
        radius = 0.001f;

    return radius;
}

float limitSegments(int segments)
{
    if (segments < 3)
        segments = 3;

    return segments;
}
} // namespace

Round::Round(float radius, int segments) : m_Radius(limitRadius(radius)), m_Segments(limitSegments(segments))
{
}

float Round::getRadius() const
{
    return m_Radius;
}

int Round::getNbrSegments() const
{
    return m_Segments;
}

void Round::setRadius(float radius)
{
    m_Radius = limitRadius(radius);
}

void Round::setNbrSegments(int segments)
{
    m_Segments = limitSegments(segments);
}
