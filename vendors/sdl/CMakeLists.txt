if(EMSCRIPTEN)
  # Same approach as imgui's example_sdl3_wgpu: SDL3 from Emscripten ports, not FetchContent.
  add_library(SDL3_emscripten INTERFACE)
  add_library(SDL3::SDL3 ALIAS SDL3_emscripten)
  target_compile_options(SDL3_emscripten INTERFACE "-sUSE_SDL=3")
  target_link_options(SDL3_emscripten INTERFACE "-sUSE_SDL=3")
else()
  include(FetchContent)

  # SDL sets WINDOWS inside SDL_DetectCMakePlatform(); with FetchContent the probe
  # block guarded by if(NOT WINDOWS) can still run, writing HAVE_SIGACTION /
  # HAVE_SYSCONF into the top-level CMake cache. That breaks Windows clang-cl
  # (incomplete struct sigaction, no unistd.h). Preseed WINDOWS in this scope
  # so the subdirectory inherits it, and drop stale HAVE_* entries from older
  # configures.
  if(WIN32)
    set(WINDOWS TRUE)
    unset(HAVE_SIGACTION CACHE)
    unset(HAVE_SA_SIGACTION CACHE)
    unset(HAVE_SYSCONF CACHE)
  endif()

  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.2
  )

  FetchContent_MakeAvailable(SDL3)
endif()
