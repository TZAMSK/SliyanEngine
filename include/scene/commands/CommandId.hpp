#pragma once

enum class CommandId
{
    // Shape creation
    OpenAddShapeDialog,
    CloseAddShapeDialog,
    AddTriangleMode,
    AddRectangleMode,
    AddCubeMode,
    AddCircleMode,

    // Camera
    CameraIsometric,
    CameraTop,
    CameraFront,
    CameraSide,

    // Gizmo
    GizmoTranslation,
    GizmoRotation,
    GizmoScale,
};
