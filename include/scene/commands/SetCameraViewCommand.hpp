#pragma once

#include "scene/commands/ICommand.hpp"

#include <functional>
#include <memory>

class Camera;
class ICameraViewStrategy;

class SetCameraViewCommand : public ICommand
{
  public:
    using StrategyFactory = std::function<std::unique_ptr<ICameraViewStrategy>()>;

    SetCameraViewCommand(Camera &camera, StrategyFactory strategyFactory);

    void execute() override;

  private:
    Camera &camera;
    StrategyFactory strategyFactory;
};
