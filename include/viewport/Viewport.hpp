#pragma once

#include "scene/shapes/Triangle.hpp"

#include <vector>

class Viewport
{
  public:
    bool init();
    void shutdown();

    void renderScene();

    void addTriangle(float x, float y);

    float *getClearColor();
    float *getFragColor();

    void resizeFramebuffer(int width, int height);
    unsigned int getColorTexture() const;
    int getFramebufferWidth() const;
    int getFramebufferHeight() const;

  private:
    void createFramebuffer(int width, int height);
    void destroyFramebuffer();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int shaderProgram = 0;

    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
    unsigned int rbo = 0;

    int framebufferWidth = 1;
    int framebufferHeight = 1;

    float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    float fragColor[4] = {1.0f, 0.5f, 0.2f, 1.0f};

    std::vector<Triangle> triangles;
};
