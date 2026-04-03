#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class AddCubeModeCommand : public ICommand
{
  public:
    explicit AddCubeModeCommand(Gui &gui);
    void execute() override;

  private:
    Gui &gui;
};
