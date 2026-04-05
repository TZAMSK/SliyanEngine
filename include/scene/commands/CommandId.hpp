#pragma once

enum class CommandId
{
    // Shape creation
    OpenAddShapeDialog,
    CloseAddShapeDialog,
    AddTriangleMode,
    AddRectangleMode,
    AddCircleMode,
    AddCubeMode,
    AddSphereMode,

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
