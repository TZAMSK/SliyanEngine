#include "gui/Gui.hpp"

#include "viewport/Viewport.hpp"
#include "console/Message.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{
constexpr float TOOLBAR_HEIGHT = 60.0f;
constexpr float CONSOLE_HEIGHT = 120.0f;
constexpr float SIDE_PANEL_WIDTH = 260.0f;
} // namespace

Gui::Gui() : message(std::make_unique<Message>(""))
{
}

Gui::~Gui() = default;

Message &Gui::getMessage()
{
    return *message;
}

const Message &Gui::getMessage() const
{
    return *message;
}

void Gui::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Gui::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

bool Gui::getAddShapeEnabled() const
{
    return addShapeEnabled;
}

void Gui::setAddShapeEnabled(bool value)
{
    addShapeEnabled = value;
}

void Gui::setShowAddShape(bool value)
{
    showAddShapeWindow = value;
}

bool Gui::getShowAddShape() const
{
    return showAddShapeWindow;
}

ImVec2 Gui::getViewportPos() const
{
    return viewportPos;
}

ImVec2 Gui::getViewportSize() const
{
    return viewportSize;
}

bool Gui::isMouseInsideViewport() const
{
    ImVec2 mouse = ImGui::GetMousePos();

    return mouse.x >= viewportPos.x && mouse.y >= viewportPos.y && mouse.x < viewportPos.x + viewportSize.x &&
           mouse.y < viewportPos.y + viewportSize.y;
}

void Gui::draw(Viewport &viewport)
{
    drawMenuBar();
    drawToolbar();
    drawLeftPanel();
    drawRightPanel(viewport);
    drawViewport(viewport);
    drawConsole();
    drawPopups();
}

void Gui::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::drawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Demo Window", nullptr, &showDemoWindow);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Gui::drawToolbar()
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float menuBarHeight = ImGui::GetFrameHeight();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(mainViewport->WorkSize.x, TOOLBAR_HEIGHT));

    ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Button("Select");
    ImGui::SameLine();
    ImGui::Button("Move");
    ImGui::SameLine();
    ImGui::Button("Rotate");
    ImGui::SameLine();
    ImGui::Button("Scale");

    ImGui::End();
}

void Gui::drawLeftPanel()
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(ImVec2(SIDE_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - CONSOLE_HEIGHT));

    ImGui::Begin("Left Panel", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Tools / Information");
    ImGui::Separator();
    ImGui::Text("Object list can go here.");
    ImGui::Text("Selection details can go here.");

    if (ImGui::Button("Add Shape"))
    {
        setShowAddShape(true);
    }

    ImGui::End();
}

void Gui::drawRightPanel(Viewport &viewport)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + mainViewport->WorkSize.x - SIDE_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(ImVec2(SIDE_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - CONSOLE_HEIGHT));

    ImGui::Begin("Right Panel", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Viewport Settings");
    ImGui::Separator();
    ImGui::ColorEdit3("Background", viewport.getClearColor());
    ImGui::ColorEdit3("Shape Color", viewport.getFragColor());

    ImGui::End();
}

void Gui::drawViewport(Viewport &viewport)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + SIDE_PANEL_WIDTH, mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(mainViewport->WorkSize.x - SIDE_PANEL_WIDTH * 2, mainViewport->WorkSize.y - topOffset - CONSOLE_HEIGHT));

    ImGui::Begin("Viewport", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImVec2 avail = ImGui::GetContentRegionAvail();
    int newWidth = static_cast<int>(avail.x);
    int newHeight = static_cast<int>(avail.y);

    if (newWidth > 0 && newHeight > 0)
    {
        viewport.resizeFramebuffer(newWidth, newHeight);
    }

    viewportPos = ImGui::GetCursorScreenPos();
    viewportSize = avail;

    ImGui::Image((ImTextureID)(intptr_t)viewport.getColorTexture(), avail, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void Gui::drawConsole()
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(
        ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + mainViewport->WorkSize.y - CONSOLE_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(mainViewport->WorkSize.x, CONSOLE_HEIGHT));

    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Logs...");
    ImGui::SameLine();
    if (ImGui::Button("bin"))
    {
        message->clearMessages();
    };

    ImGui::Separator();
    ImGui::Text(message->getMessage().data());

    ImGui::End();
}

void Gui::drawPopups()
{
    if (showAddShapeWindow)
    {
        drawAddShape();
    }

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}

void Gui::drawAddShape()
{
    ImGui::Begin("Add Shape", &showAddShapeWindow);

    ImGui::Text("Choose a shape to add:");
    ImGui::Separator();

    if (ImGui::Button("Triangle"))
    {
        setAddShapeEnabled(true);
        setShowAddShape(false);
    }

    ImGui::End();
}
