#pragma once

class Application;
class Gizmo;

class GizmoRenderer
{
  public:
    bool init();
    void draw(Application &app, Gizmo &gizmo) const;
    void shutdown();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;

  public:
    mutable bool m_wasUsingLastFrame = false;
    mutable float m_initialCircleRadius = 0.0f;
};
