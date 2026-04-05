#include "scene/commands/AddSphereModeCommand.hpp"

#include "gui/Gui.hpp"

AddSphereModeCommand::AddSphereModeCommand(Gui &gui) : gui(gui)
{
}

void AddSphereModeCommand::execute()
{
    gui.armSpherePlacement();
    gui.appendLog("Sphere placement armed. Click inside the viewport.\n");
}
