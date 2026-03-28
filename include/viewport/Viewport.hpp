#pragma once

class Viewport
{
  public:
    bool init();
    float *getClearColor();
    float *getFragColor();
    void renderScene();
    void shutdown();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int shaderProgram = 0;

    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    float fragColor[4] = {1.0f, 0.5f, 0.2f, 1.0f};
};
