#include "scene/commands/DeleteShapeCommand.hpp"

#include "gui/Gui.hpp"

DeleteShapeCommand::DeleteShapeCommand(Gui &gui) : gui(gui)
{
}

void DeleteShapeCommand::execute()
{
    gui.armCirclePlacement();
    gui.appendLog("Circle placement armed. Click inside the viewport.\n");
}
