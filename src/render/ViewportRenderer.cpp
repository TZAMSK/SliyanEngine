#include "render/ViewportRenderer.hpp"

#include "scene/Scene.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vColor = aColor;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
)";
} // namespace

bool ViewportRenderer::init()
{
    if (!shader.create(vertexShaderSource, fragmentShaderSource))
    {
        return false;
    }

    if (!gridRenderer.init())
    {
        return false;
    }

    const float triangleVertices[] = {
        0.0f,  1.0f,  0.0f,  0.95f, 0.55f, 0.20f, -1.0f, -1.0f, 0.0f,
        0.95f, 0.55f, 0.20f, 1.0f,  -1.0f, 0.0f,  0.95f, 0.55f, 0.20f,
    };

    glGenVertexArrays(1, &triangleVao);
    glGenBuffers(1, &triangleVbo);

    glBindVertexArray(triangleVao);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return createFramebuffer(framebufferWidth, framebufferHeight);
}

bool ViewportRenderer::createFramebuffer(int width, int height)
{
    framebufferWidth = width;
    framebufferHeight = height;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferWidth, framebufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    const bool isComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return isComplete;
}

void ViewportRenderer::destroyFramebuffer()
{
    if (rbo)
    {
        glDeleteRenderbuffers(1, &rbo);
        rbo = 0;
    }

    if (colorTexture)
    {
        glDeleteTextures(1, &colorTexture);
        colorTexture = 0;
    }

    if (fbo)
    {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
}

void ViewportRenderer::resizeFramebuffer(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (width == framebufferWidth && height == framebufferHeight)
    {
        return;
    }

    destroyFramebuffer();
    createFramebuffer(width, height);
}

void ViewportRenderer::render(const Scene &scene)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    const glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 view =
        glm::lookAt(scene.getCamera().getPosition(), scene.getCamera().getTarget(), scene.getCamera().getUp());
    const glm::mat4 projection =
        glm::perspective(glm::radians(scene.getCamera().getFovDegrees()),
                         static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), 0.1f, 200.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    gridRenderer.draw();

    glBindVertexArray(triangleVao);

    for (const Triangle &triangle : scene.getTriangles())
    {
        const glm::mat4 triangleModel = glm::translate(glm::mat4(1.0f), triangle.getPosition());
        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(triangleModel));
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportRenderer::shutdown()
{
    destroyFramebuffer();
    gridRenderer.shutdown();
    shader.destroy();

    if (triangleVbo)
    {
        glDeleteBuffers(1, &triangleVbo);
        triangleVbo = 0;
    }

    if (triangleVao)
    {
        glDeleteVertexArrays(1, &triangleVao);
        triangleVao = 0;
    }
}

unsigned int ViewportRenderer::getColorTexture() const
{
    return colorTexture;
}

int ViewportRenderer::getWidth() const
{
    return framebufferWidth;
}

int ViewportRenderer::getHeight() const
{
    return framebufferHeight;
}

float *ViewportRenderer::backgroundColor()
{
    return clearColor;
}
