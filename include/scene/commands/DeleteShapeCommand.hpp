#pragma once

#include "scene/commands/ICommand.hpp"

class Gui;

class DeleteShapeCommand : public ICommand
{
  public:
    explicit DeleteShapeCommand(Gui &gui);
    void execute() override;

  private:
    Gui &gui;
};
