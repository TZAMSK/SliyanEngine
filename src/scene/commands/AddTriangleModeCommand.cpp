#include "scene/commands/AddTriangleModeCommand.hpp"

#include "gui/Gui.hpp"

AddTriangleModeCommand::AddTriangleModeCommand(Gui &gui) : gui(gui)
{
}

void AddTriangleModeCommand::execute()
{
    gui.armTrianglePlacement();
    gui.appendLog("Triangle placement armed. Click inside the viewport.\n");
}
