#pragma once

class GridAxisRenderer
{
  public:
    bool init();
    void draw() const;
    void shutdown();
    void drawGround() const;

  private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    int vertexCount = 0;

    unsigned int groundVao = 0;
    unsigned int groundVbo = 0;
};
