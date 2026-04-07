#pragma once

class Round
{
  public:
    Round(float radius, int segments);
    virtual ~Round() = default;

    virtual float getRadius() const;
    virtual int getNbrSegments() const;

    virtual void setRadius(float radius);
    virtual void setNbrSegments(int segments);

  protected:
    float m_Radius = 1.0f;
    int m_Segments = 16;
};
