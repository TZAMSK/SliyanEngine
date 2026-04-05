#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class AddSphereModeCommand : public ICommand
{
  public:
    explicit AddSphereModeCommand(Gui &gui);
    void execute() override;

  private:
    Gui &gui;
};
