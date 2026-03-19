cmake_minimum_required(VERSION 3.22)
project(sdlApp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(IMGUI_DAWN_DIR dawn)

add_subdirectory(vendors)
add_subdirectory(src)
