#include "core/ShaderProgram.hpp"

#include <glad/glad.h>

#include <iostream>

unsigned int ShaderProgram::compile(unsigned int shaderType, const char *source)
{
    const unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}

bool ShaderProgram::create(const char *vertexSource, const char *fragmentSource)
{
    const unsigned int vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    int success = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
        destroy();
        return false;
    }

    return true;
}

void ShaderProgram::destroy()
{
    if (programId != 0)
    {
        glDeleteProgram(programId);
        programId = 0;
    }
}

void ShaderProgram::use() const
{
    glUseProgram(programId);
}

unsigned int ShaderProgram::id() const
{
    return programId;
}
