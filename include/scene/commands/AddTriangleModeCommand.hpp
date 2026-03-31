#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class AddTriangleModeCommand : public ICommand
{
  public:
    explicit AddTriangleModeCommand(Gui &gui);

    void execute() override;

  private:
    Gui &gui;
};
