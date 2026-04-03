#pragma once

class ShaderProgram
{
  public:
    bool create(const char *vertexSource, const char *fragmentSource);
    void destroy();
    void use() const;
    unsigned int id() const;

  private:
    unsigned int compile(unsigned int shaderType, const char *source);

  private:
    unsigned int programId = 0;
};
