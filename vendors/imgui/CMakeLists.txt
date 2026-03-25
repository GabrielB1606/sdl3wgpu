include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG docking
)

FetchContent_MakeAvailable(imgui)

add_library(imgui)

target_include_directories(
  imgui
  PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)

target_sources(
  imgui
  PRIVATE
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN)

if(EMSCRIPTEN)
  target_compile_options(imgui PUBLIC "${IMGUI_EMSCRIPTEN_WEBGPU_FLAG}")
  target_link_libraries(imgui PUBLIC SDL3::SDL3)
else()
  target_link_libraries(imgui PUBLIC webgpu_dawn SDL3::SDL3)
endif()
