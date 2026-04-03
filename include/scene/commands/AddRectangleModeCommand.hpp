#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class AddRectangleModeCommand : public ICommand
{
  public:
    explicit AddRectangleModeCommand(Gui &gui);
    void execute() override;

  private:
    Gui &gui;
};
