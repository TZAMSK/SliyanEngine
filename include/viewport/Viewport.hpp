#pragma once

class Viewport
{
  public:
    bool init();
    void renderScene();
    void drawImGuiWindow();
    void shutdown();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int shaderProgram = 0;

    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
};
