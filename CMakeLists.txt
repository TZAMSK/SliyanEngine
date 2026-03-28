cmake_minimum_required(VERSION 3.20)
project(TestGLFW LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(test src/main.cpp)
target_link_libraries(test PRIVATE glfw OpenGL::GL)
