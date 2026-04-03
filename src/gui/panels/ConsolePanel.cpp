#include "gui/panels/ConsolePanel.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"

#include "imgui.h"

void drawConsolePanel(Gui &gui)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x,
                                   mainViewport->WorkPos.y + mainViewport->WorkSize.y - GuiLayout::CONSOLE_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(mainViewport->WorkSize.x, GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Clean"))
    {
        gui.clearConsole();
    }

    ImGui::TextWrapped("%s", gui.logBuffer().c_str());

    ImGui::End();
}
