#pragma once

#include "scene/commands/ICommand.hpp"

#include <functional>
#include <memory>

class Gizmo;
class IGizmoStrategy;

class SetGizmoCommand : public ICommand
{
  public:
    using StrategyFactory = std::function<std::unique_ptr<IGizmoStrategy>()>;

    SetGizmoCommand(Gizmo &gizmo, StrategyFactory strategyFactory);

    void execute() override;

  private:
    Gizmo &gizmo;
    StrategyFactory strategyFactory;
};
