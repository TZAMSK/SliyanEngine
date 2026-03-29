#pragma once

class Gui;
class Viewport;
struct GLFWwindow;

class Application
{
  public:
    int run();
    void handleMouseClick(double mouseX, double mouseY);
    Gui *getGui();

  private:
    bool init();
    void loop();
    void shutdown();

  private:
    GLFWwindow *window = nullptr;
    Gui *gui = nullptr;
    Viewport *viewport = nullptr;
};
