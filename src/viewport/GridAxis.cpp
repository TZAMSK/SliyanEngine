#include "viewport/GridAxis.hpp"

#include <glad/glad.h>

#include <vector>

void drawGridAxis(unsigned int shaderProgram)
{
    static unsigned int gridVAO = 0;
    static unsigned int gridVBO = 0;
    static int vertexCount = 0;

    if (gridVAO == 0)
    {
        std::vector<float> vertices;

        const int halfSize = 30;
        const float step = 0.777f;

        const float minorR = 0.35f;
        const float minorG = 0.35f;
        const float minorB = 0.35f;

        const float majorR = 0.50f;
        const float majorG = 0.50f;
        const float majorB = 0.50f;

        for (int i = -halfSize; i <= halfSize; ++i)
        {
            float coord = i * step;

            bool isCenter = (i == 0);
            bool isMajor = (i % 5 == 0);

            float lineR = minorR;
            float lineG = minorG;
            float lineB = minorB;

            if (isMajor)
            {
                lineR = majorR;
                lineG = majorG;
                lineB = majorB;
            }

            if (isCenter)
            {
                vertices.insert(vertices.end(), {-halfSize * step, coord, 0.0f, 0.0f, 1.0f, 0.0f, halfSize * step,
                                                 coord, 0.0f, 0.0f, 1.0f, 0.0f});
            }
            else
            {
                vertices.insert(vertices.end(), {-halfSize * step, coord, 0.0f, lineR, lineG, lineB, halfSize * step,
                                                 coord, 0.0f, lineR, lineG, lineB});
            }

            if (isCenter)
            {
                vertices.insert(vertices.end(), {coord, -halfSize * step, 0.0f, 1.0f, 0.0f, 0.0f, coord,
                                                 halfSize * step, 0.0f, 1.0f, 0.0f, 0.0f});
            }
            else
            {
                vertices.insert(vertices.end(), {coord, -halfSize * step, 0.0f, lineR, lineG, lineB, coord,
                                                 halfSize * step, 0.0f, lineR, lineG, lineB});
            }
        }

        vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.0f, 0.4f, 1.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.4f, 1.0f});

        vertexCount = static_cast<int>(vertices.size() / 6);

        glGenVertexArrays(1, &gridVAO);
        glGenBuffers(1, &gridVBO);

        glBindVertexArray(gridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    glUseProgram(shaderProgram);
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}
