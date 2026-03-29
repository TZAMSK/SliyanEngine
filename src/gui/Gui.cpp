#include "gui/Gui.hpp"

#include "viewport/Viewport.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Gui::init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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

void Gui::setShowAddShape(bool value)
{
    showAddShapeWindow = value;
}

void Gui::draw(Viewport &viewport)
{
    drawMenuBar();
    drawToolbar();
    drawColorPicker(viewport);
    drawInspector();
    drawConsole();

    if (showAddShapeWindow)
    {
        drawAddShape();
    }

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
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
            ImGui::MenuItem("Demo Window", NULL, &showDemoWindow);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Gui::drawToolbar()
{
    ImGui::Begin("Toolbar");

    ImGui::Button("Select");
    ImGui::Button("Move");
    ImGui::Button("Rotate");
    ImGui::Button("Scale");

    ImGui::End();
}

void Gui::drawColorPicker(Viewport &viewport)
{
    ImGui::Begin("Viewport");
    ImGui::Text("This is the viewport panel.");
    ImGui::ColorEdit3("Background", viewport.getClearColor());
    ImGui::ColorEdit3("Color", viewport.getFragColor());
    ImGui::End();
}

void Gui::drawInspector()
{
    ImGui::Begin("Inspector");
    ImGui::Text("Object properties will go here.");
    ImGui::End();
}

void Gui::drawConsole()
{
    ImGui::Begin("Console");
    ImGui::Text("Logs...");
    ImGui::End();
}

void Gui::drawAddShape()
{
    ImGui::Begin("Add Shape", &showAddShapeWindow);

    ImGui::Button("Triangle");
    ImGui::Button("Move");
    ImGui::Button("Rotate");
    ImGui::Button("Scale");

    ImGui::End();
}
