cmake_minimum_required(VERSION 3.22)
project(sdlApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(IMGUI_DAWN_DIR vendors/dawn)

add_subdirectory(vendors)
add_subdirectory(src)
