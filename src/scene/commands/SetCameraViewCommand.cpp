#include "scene/commands/SetCameraViewCommand.hpp"

#include "scene/camera/Camera.hpp"
#include "scene/camera/ICameraViewStrategy.hpp"

SetCameraViewCommand::SetCameraViewCommand(Camera &camera, StrategyFactory strategyFactory)
    : camera(camera), strategyFactory(std::move(strategyFactory))
{
}

void SetCameraViewCommand::execute()
{
    camera.applyStrategy(strategyFactory());
}
