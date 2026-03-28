#pragma once

struct GLFWwindow;

class Gui;
class Viewport;

class Application
{
  public:
    int run();

  private:
    bool init();
    void loop();
    void shutdown();

  private:
    GLFWwindow *window = nullptr;

    Gui *gui = nullptr;
    Viewport *viewport = nullptr;
};
