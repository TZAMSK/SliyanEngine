#include "scene/commands/AddCircleModeCommand.hpp"

#include "gui/Gui.hpp"

AddCircleModeCommand::AddCircleModeCommand(Gui &gui) : gui(gui)
{
}

void AddCircleModeCommand::execute()
{
    gui.armCirclePlacement();
    gui.appendLog("Circle placement armed. Click inside the viewport.\n");
}
