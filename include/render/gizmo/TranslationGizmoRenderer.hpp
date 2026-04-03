#pragma once

class Application;

class TranslationGizmoRenderer
{
  public:
    bool init();
    void draw(Application &app) const;
    void shutdown();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
};
