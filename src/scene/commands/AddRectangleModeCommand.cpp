#include "scene/commands/AddRectangleModeCommand.hpp"

#include "gui/Gui.hpp"

AddRectangleModeCommand::AddRectangleModeCommand(Gui &gui) : gui(gui)
{
}

void AddRectangleModeCommand::execute()
{
    gui.armRectanglePlacement();
    gui.appendLog("Rectangle placement armed. Click inside the viewport.\n");
}
