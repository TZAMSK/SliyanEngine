#include "scene/commands/OpenAddShapeDialogCommand.hpp"

#include "gui/Gui.hpp"

OpenAddShapeDialogCommand::OpenAddShapeDialogCommand(Gui &gui) : gui(gui)
{
}

void OpenAddShapeDialogCommand::execute()
{
    gui.openAddShapeDialog();
}
