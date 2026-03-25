cmake_minimum_required(VERSION 3.22)
project(sdlApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EMSCRIPTEN)
  # Matches imgui examples/example_sdl3_wgpu (SDL3 + WebGPU in the browser).
  if(DEFINED EMSCRIPTEN_VERSION AND EMSCRIPTEN_VERSION VERSION_LESS "4.0.15")
    message(FATAL_ERROR "Emscripten >= 4.0.15 is required for SDL3 + WebGPU (see imgui example_sdl3_wgpu).")
  endif()
  set(IMGUI_EMSCRIPTEN_WEBGPU_FLAG
      "--use-port=emdawnwebgpu"
      CACHE STRING
            "Emscripten WebGPU port flag. Override with e.g. --use-port=path/to/emdawnwebgpu.port.py for a pinned Dawn package."
  )
  add_compile_options(-sDISABLE_EXCEPTION_CATCHING=1 -DIMGUI_DISABLE_FILE_FUNCTIONS=1)
else()
  set(IMGUI_DAWN_DIR
      "${CMAKE_SOURCE_DIR}/vendors/dawn"
      CACHE PATH "Path to Dawn sources (native desktop builds)")
endif()

add_subdirectory(vendors)
add_subdirectory(src)
