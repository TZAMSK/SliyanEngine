#include "app/Application.hpp"

#include "scene/camera/views/FrontViewStrategy.hpp"
#include "scene/camera/views/IsometricViewStrategy.hpp"
#include "scene/camera/views/SideViewStrategy.hpp"
#include "scene/camera/views/TopViewStrategy.hpp"
#include "scene/commands/AddTriangleModeCommand.hpp"
#include "scene/commands/DisableAddShapeModeCommand.hpp"
#include "scene/commands/OpenAddShapeDialogCommand.hpp"
#include "scene/commands/SetCameraViewCommand.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

#include <sstream>

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

void Application::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

bool Application::init()
{
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, "SliyanEngine", monitor, NULL);

    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    inputHandler.attach(window, this);

    if (!renderer.init())
    {
        return false;
    }

    gui.init(window);
    buildCommands();
    gui.setLog("Ready.\n");
    return true;
}

void Application::buildCommands()
{
    commands.emplace(CommandId::OpenAddShapeDialog, std::make_unique<OpenAddShapeDialogCommand>(gui));
    commands.emplace(CommandId::CloseAddShapeDialog, std::make_unique<DisableAddShapeModeCommand>(gui));
    commands.emplace(CommandId::AddTriangleMode, std::make_unique<AddTriangleModeCommand>(gui));

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
}

void Application::executeCommand(CommandId id)
{
    const auto it = commands.find(id);
    if (it != commands.end() && it->second)
    {
        it->second->execute();
    }
}

void Application::onViewportClicked(double mouseX, double mouseY)
{
    const ImVec2 viewportPos = gui.getViewportPos();
    const ImVec2 viewportSize = gui.getViewportSize();

    if (viewportSize.x <= 0.0f || viewportSize.y <= 0.0f)
    {
        return;
    }

    const float localX = static_cast<float>(mouseX) - viewportPos.x;
    const float localY = static_cast<float>(mouseY) - viewportPos.y;

    const float ndcX = (2.0f * localX) / viewportSize.x - 1.0f;
    const float ndcY = 1.0f - (2.0f * localY) / viewportSize.y;
    const float aspect = viewportSize.x / viewportSize.y;

    const glm::mat4 projection =
        glm::perspective(glm::radians(scene.getCamera().getFovDegrees()), aspect, 0.1f, 200.0f);
    const glm::mat4 view =
        glm::lookAt(scene.getCamera().getPosition(), scene.getCamera().getTarget(), scene.getCamera().getUp());
    const glm::mat4 inverseVP = glm::inverse(projection * view);

    const glm::vec4 nearPoint = inverseVP * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    const glm::vec4 farPoint = inverseVP * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    const glm::vec3 rayOrigin = glm::vec3(nearPoint) / nearPoint.w;
    const glm::vec3 rayEnd = glm::vec3(farPoint) / farPoint.w;
    const glm::vec3 rayDirection = glm::normalize(rayEnd - rayOrigin);

    if (glm::abs(rayDirection.z) < 0.0001f)
    {
        return;
    }

    const float t = -rayOrigin.z / rayDirection.z;
    const glm::vec3 worldPoint = rayOrigin + rayDirection * t;

    scene.addTriangleAt(worldPoint, glm::vec4(0.95f, 0.55f, 0.20f, 1.0f));

    std::ostringstream log;
    log << "Added triangle at world position: (" << worldPoint.x << ", " << worldPoint.y << ", " << worldPoint.z
        << ")\n";
    gui.appendLog(log.str());
}

void Application::requestClose()
{
    if (window)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::renderMainWindow()
{
    renderer.render(scene);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int displayWidth = 0;
    int displayHeight = 0;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);

    glClearColor(0.08f, 0.08f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gui.beginFrame();
    gui.draw(*this);
    gui.endFrame();
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
