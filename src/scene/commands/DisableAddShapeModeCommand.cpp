#include "scene/commands/DisableAddShapeModeCommand.hpp"

#include "gui/Gui.hpp"

DisableAddShapeModeCommand::DisableAddShapeModeCommand(Gui &gui) : gui(gui)
{
}

void DisableAddShapeModeCommand::execute()
{
    gui.disarmTrianglePlacement();
}
