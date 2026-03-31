#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class OpenAddShapeDialogCommand : public ICommand
{
  public:
    explicit OpenAddShapeDialogCommand(Gui &gui);

    void execute() override;

  private:
    Gui &gui;
};
