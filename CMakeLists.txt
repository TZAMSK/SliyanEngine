cmake_minimum_required(VERSION 3.20)
project(SliyanEngine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

add_library(imguizmo
    external/imguizmo/ImGuizmo.cpp
)

target_include_directories(imguizmo PUBLIC
    external/imguizmo
)

target_link_libraries(imguizmo PUBLIC
    imgui
)

add_executable(sliyan_engine
    src/main.cpp

    src/app/Application.cpp

    src/core/ShaderProgram.cpp

    src/gui/Gui.cpp
    src/gui/panels/AddShapePopup.cpp
    src/gui/panels/ConsolePanel.cpp
    src/gui/panels/InspectorPanel.cpp
    src/gui/panels/MenuBarPanel.cpp
    src/gui/panels/ScenePanel.cpp
    src/gui/panels/ToolbarPanel.cpp
    src/gui/panels/ViewportPanel.cpp
    src/gui/panels/GizmoPanel.cpp

    src/input/InputHandler.cpp

    src/render/GridAxisRenderer.cpp
    src/render/ViewportRenderer.cpp
    src/render/gizmo/GizmoRenderer.cpp
    src/render/gizmo/Gizmo.cpp
    src/render/gizmo/TranslationGizmoStrategy.cpp
    src/render/gizmo/RotationGizmoStrategy.cpp
    src/render/gizmo/ScaleGizmoStrategy.cpp

    src/scene/Scene.cpp
    src/scene/camera/Camera.cpp
    src/scene/camera/views/FrontViewStrategy.cpp
    src/scene/camera/views/IsometricViewStrategy.cpp
    src/scene/camera/views/SideViewStrategy.cpp
    src/scene/camera/views/TopViewStrategy.cpp

    src/scene/commands/AddTriangleModeCommand.cpp
    src/scene/commands/AddRectangleModeCommand.cpp
    src/scene/commands/AddCubeModeCommand.cpp
    src/scene/commands/DisableAddShapeModeCommand.cpp
    src/scene/commands/OpenAddShapeDialogCommand.cpp
    src/scene/commands/SetCameraViewCommand.cpp
    src/scene/commands/SetGizmoCommand.cpp

    src/scene/selection/SelectionManager.cpp

    src/scene/shapes/Shape.cpp
    src/scene/shapes/Triangle.cpp
    src/scene/shapes/Cube.cpp
    src/scene/shapes/Rectangle.cpp
    src/scene/shapes/Shape2D.cpp
    src/scene/shapes/Shape3D.cpp

    src/glad.c
)

target_include_directories(sliyan_engine PRIVATE
    include
    external/imguizmo
    external/imgui
    external/imgui/backends
)

target_compile_definitions(sliyan_engine PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(sliyan_engine PRIVATE
    imgui
    imguizmo
    glfw
    OpenGL::GL
)
