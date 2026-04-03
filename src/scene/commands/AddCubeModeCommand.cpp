#include "scene/commands/AddCubeModeCommand.hpp"

#include "gui/Gui.hpp"

AddCubeModeCommand::AddCubeModeCommand(Gui &gui) : gui(gui)
{
}

void AddCubeModeCommand::execute()
{
    gui.armCubePlacement();
    gui.appendLog("Cube placement armed. Click inside the viewport.\n");
}
