#include "app/Application.hpp"

#include "scene/camera/views/FrontViewStrategy.hpp"
#include "scene/camera/views/IsometricViewStrategy.hpp"
#include "scene/camera/views/SideViewStrategy.hpp"
#include "scene/camera/views/TopViewStrategy.hpp"
#include "scene/camera/views/OrthographicViewStrategy.hpp"

#include "render/gizmo/TranslationGizmoStrategy.hpp"
#include "render/gizmo/RotationGizmoStrategy.hpp"
#include "render/gizmo/ScaleGizmoStratgy.hpp"

#include "scene/commands/AddTriangleModeCommand.hpp"
#include "scene/commands/AddRectangleModeCommand.hpp"
#include "scene/commands/AddCircleModeCommand.hpp"
#include "scene/commands/AddCubeModeCommand.hpp"
#include "scene/commands/AddSphereModeCommand.hpp"
#include "scene/commands/DisableAddShapeModeCommand.hpp"
#include "scene/commands/OpenAddShapeDialogCommand.hpp"
#include "scene/commands/SetCameraViewCommand.hpp"
#include "scene/commands/SetGizmoCommand.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

#include <sstream>

static constexpr glm::vec4 kDefaultColor{0.608f, 0.608f, 0.608f, 1.0f};

Scene &Application::getScene()
{
    return scene;
}

Gui &Application::getGui()
{
    return gui;
}

ViewportRenderer &Application::getRenderer()
{
    return renderer;
}

SelectionManager &Application::getSelectionManager()
{
    return selectionManager;
}

InputHandler &Application::getInputHandler()
{
    return inputHandler;
}

Gizmo &Application::getGizmo()
{
    return gizmo;
}

ViewportSettings &Application::getViewportSettings()
{
    return viewportSettings;
}

const ViewportSettings &Application::getViewportSettings() const
{
    return viewportSettings;
}

void Application::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

bool Application::init()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *targetMonitor = Application::initializeMonitor(1);
    const GLFWvidmode *mode = glfwGetVideoMode(targetMonitor);
    window = glfwCreateWindow(mode->width, mode->height, "SliyanEngine", targetMonitor, nullptr);

    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return false;

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    inputHandler.attach(window, this);

    if (!renderer.init())
        return false;

    gui.init(window);
    buildCommands();
    gui.setLog("Ready.\n");
    return true;
}

void Application::buildCommands()
{
    buildAddShapeCommands();
    buildCameraCommands();
    buildGizmoCommands();
}

void Application::buildAddShapeCommands()
{
    commands.emplace(CommandId::OpenAddShapeDialog, std::make_unique<OpenAddShapeDialogCommand>(gui));
    commands.emplace(CommandId::CloseAddShapeDialog, std::make_unique<DisableAddShapeModeCommand>(gui));
    commands.emplace(CommandId::AddTriangleMode, std::make_unique<AddTriangleModeCommand>(gui));
    commands.emplace(CommandId::AddRectangleMode, std::make_unique<AddRectangleModeCommand>(gui));
    commands.emplace(CommandId::AddCircleMode, std::make_unique<AddCircleModeCommand>(gui));
    commands.emplace(CommandId::AddCubeMode, std::make_unique<AddCubeModeCommand>(gui));
    commands.emplace(CommandId::AddSphereMode, std::make_unique<AddSphereModeCommand>(gui));
}

void Application::buildCameraCommands()
{
    commands.emplace(CommandId::CameraIsometric, std::make_unique<SetCameraViewCommand>(scene.getCamera(), [] {
                         return std::make_unique<IsometricViewStrategy>();
                     }));
    commands.emplace(CommandId::CameraTop, std::make_unique<SetCameraViewCommand>(
                                               scene.getCamera(), [] { return std::make_unique<TopViewStrategy>(); }));
    commands.emplace(CommandId::CameraFront, std::make_unique<SetCameraViewCommand>(scene.getCamera(), [] {
                         return std::make_unique<FrontViewStrategy>();
                     }));
    commands.emplace(CommandId::CameraSide, std::make_unique<SetCameraViewCommand>(scene.getCamera(), [] {
                         return std::make_unique<SideViewStrategy>();
                     }));
    commands.emplace(CommandId::CameraOrthographic, std::make_unique<SetCameraViewCommand>(scene.getCamera(), [] {
                         return std::make_unique<OrthographicViewStrategy>();
                     }));
}

void Application::buildGizmoCommands()
{
    commands.emplace(CommandId::GizmoTranslation, std::make_unique<SetGizmoCommand>(gizmo, [] {
                         return std::make_unique<TranslationGizmoStrategy>();
                     }));
    commands.emplace(CommandId::GizmoRotation, std::make_unique<SetGizmoCommand>(
                                                   gizmo, [] { return std::make_unique<RotationGizmoStrategy>(); }));
    commands.emplace(CommandId::GizmoScale,
                     std::make_unique<SetGizmoCommand>(gizmo, [] { return std::make_unique<ScaleGizmoStrategy>(); }));
}

void Application::executeCommand(CommandId id)
{
    const auto it = commands.find(id);
    if (it != commands.end() && it->second)
    {
        it->second->execute();
    }
}

void Application::onViewportClicked(const glm::vec3 &worldPoint)
{
    const PlacementMode mode = gui.getPlacementMode();

    std::ostringstream log;

    if (mode == PlacementMode::Triangle)
    {
        scene.addTriangleAt(worldPoint, kDefaultColor);
        log << "Added Triangle at (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")\n";
    }
    else if (mode == PlacementMode::Rectangle)
    {
        scene.addRectangleAt(worldPoint, kDefaultColor);
        log << "Added Rectangle at (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")\n";
    }
    else if (mode == PlacementMode::Circle)
    {
        scene.addCircleAt(worldPoint, kDefaultColor, 2.0f, 10);
        log << "Added Circle at (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")\n";
    }
    else if (mode == PlacementMode::Cube)
    {
        scene.addCubeAt(worldPoint, kDefaultColor);
        log << "Added Cube at (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")\n";
    }
    else if (mode == PlacementMode::Sphere)
    {
        scene.addSphereAt(worldPoint, kDefaultColor, 2.0f, 10);
        log << "Added Sphere at (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z << ")\n";
    }

    gui.appendLog(log.str());
    executeCommand(CommandId::CloseAddShapeDialog);
}

void Application::requestClose()
{
    if (window)
        glfwSetWindowShouldClose(window, true);
}

void Application::renderMainWindow()
{
    renderer.render(*this);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int displayWidth = 0, displayHeight = 0;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);

    glClearColor(0.08f, 0.08f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gui.beginFrame();
    gui.draw(*this);
    gui.endFrame();
}

GLFWmonitor *Application::initializeMonitor(int monitorIndex)
{
    int count = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    if (!monitors || count == 0)
        return nullptr;

    if (monitorIndex >= 0 && monitorIndex < count)
        return monitors[monitorIndex];

    return glfwGetPrimaryMonitor();
}

void Application::loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        renderMainWindow();
        glfwSwapBuffers(window);
    }
}

void Application::shutdown()
{
    gui.shutdown();
    renderer.shutdown();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

int Application::run()
{
    if (!init())
    {
        shutdown();
        return -1;
    }

    loop();
    shutdown();
    return 0;
}
