#pragma once

enum class CommandId
{
    // Shape creation
    OpenAddShapeDialog,
    CloseAddShapeDialog,
    AddTriangleMode,
    AddRectangleMode,
    AddCubeMode,

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
