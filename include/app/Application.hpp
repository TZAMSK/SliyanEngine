#pragma once

#include "gui/Gui.hpp"
#include "input/InputHandler.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"
#include "scene/camera/Camera.hpp"
#include "scene/commands/CommandId.hpp"
#include "scene/commands/ICommand.hpp"
#include "scene/selection/SelectionManager.hpp"
#include "render/gizmo/Gizmo.hpp"

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include <map>
#include <memory>

class ICommand;

class Application
{
  public:
    int run();

    Scene &getScene();
    Gui &getGui();
    ViewportRenderer &getRenderer();
    SelectionManager &getSelectionManager();
    InputHandler &getInputHandler();
    Gizmo &getGizmo();

    void executeCommand(CommandId id);
    void onViewportClicked(const glm::vec3 &worldPoint);
    void requestClose();

  private:
    bool init();
    void buildCommands();
    void buildAddShapeCommands();
    void buildCameraCommands();
    void buildGizmoCommands();

    void loop();
    void shutdown();
    void renderMainWindow();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static GLFWmonitor *initializeMonitor(int monitorIndex);

    GLFWwindow *window = nullptr;

    Scene scene;
    Gui gui;
    ViewportRenderer renderer;
    SelectionManager selectionManager;
    InputHandler inputHandler;
    Camera camera;
    Gizmo gizmo;

    std::map<CommandId, std::unique_ptr<ICommand>> commands;
};
