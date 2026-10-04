#include "core/ShaderProgram.hpp"

#include <glad/glad.h>

#include <iostream>
#include <vector>

unsigned int ShaderProgram::compile(unsigned int shaderType, const char *source)
{
    const unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> infoLog(logLength > 1 ? logLength : 1, '\0');
        glGetShaderInfoLog(shader, static_cast<int>(infoLog.size()), nullptr, infoLog.data());

        std::cerr << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation failed:\n"
                  << infoLog.data() << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ShaderProgram::create(const char *vertexSource, const char *fragmentSource)
{
    // Avoid leaking a previously created program
    destroy();

    const unsigned int vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0)
        return false;

    const unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    int success = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success)
    {
        int logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> infoLog(logLength > 1 ? logLength : 1, '\0');
        glGetProgramInfoLog(programId, static_cast<int>(infoLog.size()), nullptr, infoLog.data());

        std::cerr << "Program linking failed:\n" << infoLog.data() << std::endl;
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
