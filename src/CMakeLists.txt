set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(myapp debug_gui.cpp main.cpp)
target_include_directories(
  myapp
  PUBLIC
  ../include
)
target_link_libraries(
  myapp
  PRIVATE
  imgui
)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  target_link_options(
    myapp
    PRIVATE
    "${IMGUI_EMSCRIPTEN_WEBGPU_FLAG}"
    "-sUSE_SDL=3"
    "-sWASM=1"
    "-sASYNCIFY=1"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sNO_EXIT_RUNTIME=0"
    "-sASSERTIONS=1"
    "-sDISABLE_EXCEPTION_CATCHING=1"
    "--shell-file=${CMAKE_SOURCE_DIR}/libs/emscripten/shell_minimal.html"
  )
  set_target_properties(myapp PROPERTIES OUTPUT_NAME "index")
endif()
