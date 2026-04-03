#include "scene/commands/SetGizmoCommand.hpp"

#include "render/gizmo/Gizmo.hpp"
#include "render/gizmo/IGizmoStrategy.hpp"

SetGizmoCommand::SetGizmoCommand(Gizmo &gizmo, StrategyFactory strategyFactory)
    : gizmo(gizmo), strategyFactory(std::move(strategyFactory))
{
}

void SetGizmoCommand::execute()
{
    gizmo.applyStrategy(strategyFactory());
}
