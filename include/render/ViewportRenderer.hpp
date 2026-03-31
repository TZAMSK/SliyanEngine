#pragma once

#include "core/ShaderProgram.hpp"
#include "render/GridAxisRenderer.hpp"

class Scene;

class ViewportRenderer
{
  public:
    bool init();
    void shutdown();

    void resizeFramebuffer(int width, int height);
    void render(const Scene &scene);

    unsigned int getColorTexture() const;
    int getWidth() const;
    int getHeight() const;

    float *backgroundColor();

  private:
    bool createFramebuffer(int width, int height);
    void destroyFramebuffer();

  private:
    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
    unsigned int rbo = 0;
    int framebufferWidth = 1280;
    int framebufferHeight = 720;

    float clearColor[4] = {0.10f, 0.10f, 0.12f, 1.0f};

    unsigned int triangleVao = 0;
    unsigned int triangleVbo = 0;

    ShaderProgram shader;
    GridAxisRenderer gridRenderer;
};
