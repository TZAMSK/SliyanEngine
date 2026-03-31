#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class DisableAddShapeModeCommand : public ICommand
{
  public:
    explicit DisableAddShapeModeCommand(Gui &gui);

    void execute() override;

  private:
    Gui &gui;
};
