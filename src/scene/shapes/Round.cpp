#include "scene/shapes/Round.hpp"

Round::Round(float radius, int segments) : m_Radius(radius), m_Segments(segments)
{
    if (this->m_Radius < 0.001f)
        this->m_Radius = 0.001f;

    if (this->m_Segments < 3)
        this->m_Segments = 3;
}

float Round::getRadius() const
{
    return m_Radius;
}

void Round::setRadius(float radius)
{
    if (radius < 0.001f)
        radius = 0.001f;

    m_Radius = radius;
}

int Round::getNbrSegments() const
{
    return m_Segments;
}

void Round::setNbrSegments(int segments)
{
    if (segments < 3)
        segments = 3;

    m_Segments = segments;
}
