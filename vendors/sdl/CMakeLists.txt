if(EMSCRIPTEN)
  # Same approach as imgui's example_sdl3_wgpu: SDL3 from Emscripten ports, not FetchContent.
  add_library(SDL3_emscripten INTERFACE)
  add_library(SDL3::SDL3 ALIAS SDL3_emscripten)
  target_compile_options(SDL3_emscripten INTERFACE "-sUSE_SDL=3")
  target_link_options(SDL3_emscripten INTERFACE "-sUSE_SDL=3")
else()
  include(FetchContent)

  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.2
  )

  FetchContent_MakeAvailable(SDL3)
endif()
