#pragma once

#include "core/ShaderProgram.hpp"
#include "render/GridAxisRenderer.hpp"
#include "render/gizmo/GizmoRenderer.hpp"

#include <glad/glad.h>

class Application;

class ViewportRenderer
{
  public:
    bool init();
    void render(Application &app);
    void resizeFramebuffer(int width, int height);
    void shutdown();

    GLuint getColorTexture() const;
    GLuint getFbo() const;
    int getWidth() const;
    int getHeight() const;
    float *backgroundColor();

    GizmoRenderer &getGizmoRenderer()
    {
        return gizmoRenderer;
    }

  private:
    bool createFramebuffer(int width, int height);
    void destroyFramebuffer();

    ShaderProgram shader;
    ShaderProgram outlineShader;
    GridAxisRenderer gridRenderer;
    GizmoRenderer gizmoRenderer;

    GLuint triangleVao = 0;
    GLuint rectangleVao = 0;
    GLuint cubeVao = 0;

    GLuint fbo = 0;
    GLuint colorTexture = 0;
    GLuint idTexture = 0;
    GLuint rbo = 0;

    int framebufferWidth = 1280;
    int framebufferHeight = 720;

    float clearColor[4] = {0.18f, 0.18f, 0.18f, 1.0f};
};
