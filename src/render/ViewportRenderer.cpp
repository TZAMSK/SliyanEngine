#include "render/ViewportRenderer.hpp"

#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/shapes/2d/Triangle.hpp"
#include "scene/shapes/2d/Rectangle.hpp"
#include "scene/shapes/2d/Circle.hpp"
#include "scene/shapes/3d/Cube.hpp"
#include "scene/shapes/3d/Sphere.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "app/Application.hpp"
#include "util/readFile.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

GLuint vaoForShape(const Shape &shape)
{
    if (shape.getType() == ShapeType::Triangle)
    {
        const Triangle *triangle = dynamic_cast<const Triangle *>(&shape);
        return triangle ? triangle->getVao() : 0;
    }

    if (shape.getType() == ShapeType::Rectangle)
    {
        const Rectangle *rectangle = dynamic_cast<const Rectangle *>(&shape);
        return rectangle ? rectangle->getVao() : 0;
    }

    if (shape.getType() == ShapeType::Circle)
    {
        const Circle *circle = dynamic_cast<const Circle *>(&shape);
        return circle ? circle->getVao() : 0;
    }

    if (shape.getType() == ShapeType::Cube)
    {
        const Cube *cube = dynamic_cast<const Cube *>(&shape);
        return cube ? cube->getVao() : 0;
    }

    if (shape.getType() == ShapeType::Sphere)
    {
        const Sphere *sphere = dynamic_cast<const Sphere *>(&shape);
        return sphere ? sphere->getVao() : 0;
    }

    return 0;
}

} // namespace

bool ViewportRenderer::init()
{
    std::string vertexShaderSource = loadShaderAsString("src/core/shaders/vertexShader.vert");
    std::string fragmentShaderSource = loadShaderAsString("src/core/shaders/fragmentShader.frag");
    std::string outlineVertexShaderSource = loadShaderAsString("src/core/shaders/outlineVertexShader.vert");
    std::string outlineFragmentShaderSource = loadShaderAsString("src/core/shaders/outlineFragmentShader.frag");

    if (!shader.create(vertexShaderSource.c_str(), fragmentShaderSource.c_str()))
        return false;

    if (!outlineShader.create(outlineVertexShaderSource.c_str(), outlineFragmentShaderSource.c_str()))
        return false;

    if (!gridRenderer.init())
        return false;

    if (!gizmoRenderer.init())
        return false;

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

void ViewportRenderer::render(Application &app)
{
    const Scene &scene = app.getScene();
    const SelectionManager selection = app.getSelectionManager();
    const ViewMode viewMode = app.getViewportSettings().viewMode;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLuint clearID = 0;
    glClearBufferuiv(GL_COLOR, 1, &clearID);

    const glm::mat4 view =
        glm::lookAt(scene.getCamera().getPosition(), scene.getCamera().getTarget(), scene.getCamera().getUp());

    const glm::mat4 proj = app.getScene().getCamera().getProjection(ImVec2(framebufferWidth, framebufferHeight));

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
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
        const bool selected = (shape.getId() == selection.selectedId());
        const bool hovered = (shape.getId() == selection.hoveredId());

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(selected ? 0xFF : 0x00);

        const glm::mat4 model = shape.getTransform();
        const glm::vec4 &c = shape.getColor();

        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1ui(glGetUniformLocation(shader.id(), "objectID"), shape.getId());
        glUniform1i(glGetUniformLocation(shader.id(), "isHovered"), hovered ? 1 : 0);
        glUniform1i(glGetUniformLocation(shader.id(), "isSelected"), selected ? 1 : 0);
        glUniform3f(glGetUniformLocation(shader.id(), "shapeColor"), c.r, c.g, c.b);

        const GLuint vao = vaoForShape(shape);
        if (vao == 0)
            continue;

        glBindVertexArray(vao);

        if (viewMode == ViewMode::Wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(shape.getVertexCount()));
    }

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

            glm::mat4 outlineModel = sel->getTransform();
            outlineModel = glm::scale(outlineModel, glm::vec3(1.03f));

            glUniformMatrix4fv(glGetUniformLocation(outlineShader.id(), "model"), 1, GL_FALSE,
                               glm::value_ptr(outlineModel));

            const GLuint vao = vaoForShape(*sel);
            if (vao != 0)
            {
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sel->getVertexCount()));
            }

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
