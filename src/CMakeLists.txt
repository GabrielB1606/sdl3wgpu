set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(myapp main.cpp)
target_link_libraries(myapp
    PRIVATE
    webgpu_dawn
    SDL3::SDL3
    imgui
)
