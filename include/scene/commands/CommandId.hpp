#pragma once

enum class CommandId
{
    // Shape creation
    AddTriangleMode,
    AddRectangleMode,
    AddCircleMode,
    AddCubeMode,
    AddSphereMode,

    // Shape Deletion
    DeleteShape,

    // Camera
    CameraIsometric,
    CameraTop,
    CameraFront,
    CameraSide,
    CameraOrthographic,

    // Gizmo
    GizmoTranslation,
    GizmoRotation,
    GizmoScale,
};
