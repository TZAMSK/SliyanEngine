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
};
