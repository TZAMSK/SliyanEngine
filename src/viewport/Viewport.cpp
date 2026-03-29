#include "viewport/Viewport.hpp"

#include "imgui.h"

#include <glad/glad.h>

#include <iostream>

namespace
{
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "    gl_Position = vec4(aPos, 1.0f);\n"
                                 "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 uColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = uColor;\n"
                                   "}\n";

unsigned int createShader(unsigned int type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success = 0;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}
} // namespace

bool Viewport::init()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success = 0;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Program linking failed:\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

float *Viewport::getClearColor()
{
    return clearColor;
}

float *Viewport::getFragColor()
{
    return fragColor;
}

void Viewport::addTriangle(float x, float y)
{
    triangles.emplace_back(glm::vec3(x, y, 0.0f), glm::vec4(fragColor[0], fragColor[1], fragColor[2], fragColor[3]));
}

void Viewport::renderScene()
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    for (const Triangle &triangle : triangles)
    {
        float vertices[] = {
            triangle.getVertex(0).x + triangle.getPosition().x, triangle.getVertex(0).y + triangle.getPosition().y,
            triangle.getVertex(0).z + triangle.getPosition().z,

            triangle.getVertex(1).x + triangle.getPosition().x, triangle.getVertex(1).y + triangle.getPosition().y,
            triangle.getVertex(1).z + triangle.getPosition().z,

            triangle.getVertex(2).x + triangle.getPosition().x, triangle.getVertex(2).y + triangle.getPosition().y,
            triangle.getVertex(2).z + triangle.getPosition().z};

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
        const glm::vec4 &color = triangle.getColor();
        glUniform4f(colorLocation, color.r, color.g, color.b, color.a);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void Viewport::shutdown()
{
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

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
