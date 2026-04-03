#include "render/ViewportRenderer.hpp"

#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "app/Application.hpp"

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

uniform vec3 shapeColor;
uniform int useVertexColor;

void main()
{
    vColor = (useVertexColor == 1) ? aColor : shapeColor;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core

in vec3 vColor;

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint outObjectID;

uniform uint objectID;
uniform int isHovered;
uniform int isSelected;

void main()
{
    vec3 color = vColor;

    if (isSelected == 1 || isHovered == 1)
        color = mix(color, vec3(1.0, 0.72, 0.25), 0.05);

    outColor = vec4(color, 1.0);
    outObjectID = objectID;
}
)";

const char *outlineVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
)";

const char *outlineFragmentShaderSource = R"(
#version 330 core

layout (location = 0) out vec4 outColor;
layout (location = 1) out uint outObjectID;

void main()
{
    outColor = vec4(1.0, 0.45, 0.05, 1.0);   // vivid orange
    outObjectID = 0u;
}
)";

const float kTriangleVerts[] = {
    0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
};

const float kRectVerts[] = {
    -1.0f, -0.75f, 0.0f, 1.0f, -0.75f, 0.0f, 1.0f,  0.75f, 0.0f,

    -1.0f, -0.75f, 0.0f, 1.0f, 0.75f,  0.0f, -1.0f, 0.75f, 0.0f,
};

const float kCubeVerts[] = {
    // -Z face
    -1,
    -1,
    -1,
    1,
    -1,
    -1,
    1,
    1,
    -1,
    -1,
    -1,
    -1,
    1,
    1,
    -1,
    -1,
    1,
    -1,
    // +Z face
    -1,
    -1,
    1,
    1,
    1,
    1,
    1,
    -1,
    1,
    -1,
    -1,
    1,
    -1,
    1,
    1,
    1,
    1,
    1,
    // -Y face
    -1,
    -1,
    -1,
    -1,
    -1,
    1,
    1,
    -1,
    1,
    -1,
    -1,
    -1,
    1,
    -1,
    1,
    1,
    -1,
    -1,
    // +Y face
    -1,
    1,
    -1,
    1,
    1,
    1,
    -1,
    1,
    1,
    -1,
    1,
    -1,
    1,
    1,
    -1,
    1,
    1,
    1,
    // -X face
    -1,
    -1,
    -1,
    -1,
    1,
    -1,
    -1,
    1,
    1,
    -1,
    -1,
    -1,
    -1,
    1,
    1,
    -1,
    -1,
    1,
    // +X face
    1,
    -1,
    -1,
    1,
    1,
    1,
    1,
    1,
    -1,
    1,
    -1,
    -1,
    1,
    -1,
    1,
    1,
    1,
    1,
};

GLuint makeVao(const float *verts, GLsizeiptr byteSize)
{
    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    return vao;
}

} // namespace

bool ViewportRenderer::init()
{
    if (!shader.create(vertexShaderSource, fragmentShaderSource))
        return false;

    if (!outlineShader.create(outlineVertexShaderSource, outlineFragmentShaderSource))
        return false;

    if (!gridRenderer.init())
        return false;

    if (!gizmoRenderer.init())
        return false;

    triangleVao = makeVao(kTriangleVerts, sizeof(kTriangleVerts));
    rectangleVao = makeVao(kRectVerts, sizeof(kRectVerts));
    cubeVao = makeVao(kCubeVerts, sizeof(kCubeVerts));

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, idTexture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    const bool ok = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return ok;
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
    if (idTexture)
    {
        glDeleteTextures(1, &idTexture);
        idTexture = 0;
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
        return;
    if (width == framebufferWidth && height == framebufferHeight)
        return;
    destroyFramebuffer();
    createFramebuffer(width, height);
}

static void bindShapeVao(GLuint triangleVao, GLuint rectangleVao, GLuint cubeVao, ShapeType type)
{
    switch (type)
    {
    case ShapeType::Triangle:
        glBindVertexArray(triangleVao);
        break;
    case ShapeType::Rectangle:
        glBindVertexArray(rectangleVao);
        break;
    case ShapeType::Cube:
        glBindVertexArray(cubeVao);
        break;
    }
}

static int vertexCountFor(ShapeType type)
{
    switch (type)
    {
    case ShapeType::Triangle:
        return 3;
    case ShapeType::Rectangle:
        return 6;
    case ShapeType::Cube:
        return 36;
    }
    return 3;
}

void ViewportRenderer::render(Application &app)
{
    const Scene &scene = app.getScene();
    const SelectionManager selection = app.getSelectionManager();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    // Clear colour, depth, and stencil
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLuint clearID = 0;
    glClearBufferuiv(GL_COLOR, 1, &clearID);

    // Camera matrices
    const glm::mat4 view =
        glm::lookAt(scene.getCamera().getPosition(), scene.getCamera().getTarget(), scene.getCamera().getUp());

    const glm::mat4 proj =
        glm::perspective(glm::radians(scene.getCamera().getFovDegrees()),
                         static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), 0.1f, 200.0f);

    // Stencil
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    // Grid
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilMask(0xFF);
    glUniform1ui(glGetUniformLocation(shader.id(), "objectID"), 0);
    glUniform1i(glGetUniformLocation(shader.id(), "isHovered"), 0);
    glUniform1i(glGetUniformLocation(shader.id(), "isSelected"), 0);
    glUniform1i(glGetUniformLocation(shader.id(), "useVertexColor"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    gridRenderer.draw();
    glUniform1i(glGetUniformLocation(shader.id(), "useVertexColor"), 0);

    for (const auto &shapePtr : scene.getShapes())
    {
        const Shape &shape = *shapePtr;
        const bool selected = (shape.id == selection.selectedId());
        const bool hovered = (shape.id == selection.hoveredId());

        glStencilFunc(GL_ALWAYS, selected ? 1 : 0, 0xFF);
        glStencilMask(0xFF);

        const glm::mat4 model = shape.getTransform();
        const glm::vec4 &c = shape.getColor();

        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1ui(glGetUniformLocation(shader.id(), "objectID"), shape.id);
        glUniform1i(glGetUniformLocation(shader.id(), "isHovered"), hovered ? 1 : 0);
        glUniform1i(glGetUniformLocation(shader.id(), "isSelected"), selected ? 1 : 0);
        glUniform3f(glGetUniformLocation(shader.id(), "shapeColor"), c.r, c.g, c.b);

        bindShapeVao(triangleVao, rectangleVao, cubeVao, shape.getType());
        glDrawArrays(GL_TRIANGLES, 0, vertexCountFor(shape.getType()));
    }

    // Orange outline

    if (selection.hasSelection())
    {
        const Shape *sel = scene.findShapeById(selection.selectedId());
        if (sel)
        {
            outlineShader.use();
            glUniformMatrix4fv(glGetUniformLocation(outlineShader.id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(outlineShader.id(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            const float outlineScale = 1.05f;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), sel->getPosition());
            model = glm::scale(model, glm::vec3(outlineScale));

            glUniformMatrix4fv(glGetUniformLocation(outlineShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));

            bindShapeVao(triangleVao, rectangleVao, cubeVao, sel->getType());
            glDrawArrays(GL_TRIANGLES, 0, vertexCountFor(sel->getType()));

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }

    glDisable(GL_STENCIL_TEST);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportRenderer::shutdown()
{
    destroyFramebuffer();
    shader.destroy();
    outlineShader.destroy();
    gridRenderer.shutdown();

    auto deleteVao = [](GLuint &vao) {
        if (vao)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    };
    deleteVao(triangleVao);
    deleteVao(rectangleVao);
    deleteVao(cubeVao);
}

GLuint ViewportRenderer::getColorTexture() const
{
    return colorTexture;
}
GLuint ViewportRenderer::getFbo() const
{
    return fbo;
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
