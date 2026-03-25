if(NOT EMSCRIPTEN)
  set(DAWN_FETCH_DEPENDENCIES ON)
  add_subdirectory(dawn EXCLUDE_FROM_ALL)
endif()

add_subdirectory(sdl)
add_subdirectory(imgui)
