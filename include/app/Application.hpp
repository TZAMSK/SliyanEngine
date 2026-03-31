#pragma once

#include "gui/Gui.hpp"
#include "input/InputHandler.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"
#include "scene/commands/CommandId.hpp"
#include "scene/commands/ICommand.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_map>

class Application
{
  public:
    Application() = default;
    ~Application() = default;

    int run();

    Scene &getScene();
    Gui &getGui();
    ViewportRenderer &getRenderer();

    void executeCommand(CommandId id);
    void onViewportClicked(double mouseX, double mouseY);
    void requestClose();

  private:
    bool init();
    void loop();
    void shutdown();
    void buildCommands();
    void renderMainWindow();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

  private:
    GLFWwindow *window = nullptr;
    Scene scene;
    Gui gui;
    ViewportRenderer renderer;
    InputHandler inputHandler;
    std::unordered_map<CommandId, std::unique_ptr<ICommand>> commands;
};
