#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class AddCircleModeCommand : public ICommand
{
  public:
    explicit AddCircleModeCommand(Gui &gui);
    void execute() override;

  private:
    Gui &gui;
};
