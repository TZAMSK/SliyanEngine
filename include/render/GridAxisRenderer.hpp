#pragma once

class GridAxisRenderer
{
  public:
    bool init();
    void draw() const;
    void shutdown();

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    int vertexCount = 0;
};
