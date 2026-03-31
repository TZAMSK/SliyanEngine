#include "render/GridAxisRenderer.hpp"

#include <glad/glad.h>

#include <vector>

bool GridAxisRenderer::init()
{
    std::vector<float> vertices;

    const int halfSize = 30;
    const float step = 1.0f;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        const float coord = static_cast<float>(i) * step;
        const bool isCenter = (i == 0);
        const bool isMajor = (i % 5 == 0);

        float lineR = 0.30f;
        float lineG = 0.30f;
        float lineB = 0.30f;

        if (isMajor)
        {
            lineR = 0.42f;
            lineG = 0.42f;
            lineB = 0.42f;
        }

        if (isCenter)
        {
            vertices.insert(vertices.end(), {-halfSize * step, coord, 0.0f, 0.0f, 1.0f, 0.0f, halfSize * step, coord,
                                             0.0f, 0.0f, 1.0f, 0.0f});
            vertices.insert(vertices.end(), {coord, -halfSize * step, 0.0f, 1.0f, 0.0f, 0.0f, coord, halfSize * step,
                                             0.0f, 1.0f, 0.0f, 0.0f});
        }
        else
        {
            vertices.insert(vertices.end(), {-halfSize * step, coord, 0.0f, lineR, lineG, lineB, halfSize * step, coord,
                                             0.0f, lineR, lineG, lineB});
            vertices.insert(vertices.end(), {coord, -halfSize * step, 0.0f, lineR, lineG, lineB, coord, halfSize * step,
                                             0.0f, lineR, lineG, lineB});
        }
    }

    vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.15f, 0.45f, 1.0f, 0.0f, 0.0f, 5.0f, 0.15f, 0.45f, 1.0f});

    vertexCount = static_cast<int>(vertices.size() / 6);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return true;
}

void GridAxisRenderer::draw() const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

void GridAxisRenderer::shutdown()
{
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}
