cmake_minimum_required(VERSION 3.20)
project(SliyanRender LANGUAGES C CXX)

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

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_executable(test
    src/main.cpp
    src/app/Application.cpp
    src/gui/Gui.cpp
    src/viewport/Viewport.cpp
    src/scene/shapes/Shape.cpp
    src/scene/shapes/Triangle.cpp
    src/scene/commands/Shortcuts.cpp
    src/glad.c
)

target_include_directories(test PRIVATE
    include
)

target_link_libraries(test PRIVATE
    imgui
    glfw
    OpenGL::GL
)
