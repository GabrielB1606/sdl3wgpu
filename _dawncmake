include(FetchContent)

# Dawn: fetch its own dependencies (abseil, SPIRV-Tools, etc.)
set(DAWN_FETCH_DEPENDENCIES ON)

# Disable everything we don't need
set(DAWN_BUILD_SAMPLES OFF)
set(DAWN_BUILD_TESTS OFF)
set(DAWN_BUILD_BENCHMARKS OFF)
set(DAWN_ENABLE_INSTALL OFF)

# GPU backends: only keep D3D12 and Vulkan for Windows
set(DAWN_ENABLE_D3D11 OFF)
set(DAWN_ENABLE_D3D12 ON)
set(DAWN_ENABLE_METAL OFF)
set(DAWN_ENABLE_NULL OFF)
set(DAWN_ENABLE_DESKTOP_GL OFF)
set(DAWN_ENABLE_OPENGLES OFF)
set(DAWN_ENABLE_VULKAN ON)

# Tint shader compiler: only build the readers/writers we actually need
set(TINT_BUILD_SPV_READER OFF)
set(TINT_BUILD_SPV_WRITER ON)    # needed for Vulkan
set(TINT_BUILD_WGSL_READER ON)   # needed to parse WGSL shaders
set(TINT_BUILD_WGSL_WRITER OFF)
set(TINT_BUILD_GLSL_WRITER OFF)
set(TINT_BUILD_GLSL_VALIDATOR OFF)
set(TINT_BUILD_HLSL_WRITER ON)   # needed for D3D12
set(TINT_BUILD_MSL_WRITER OFF)
set(TINT_BUILD_TESTS OFF)
set(TINT_BUILD_CMD_TOOLS OFF)

FetchContent_Declare(
  dawn
  GIT_REPOSITORY https://github.com/google/dawn
  GIT_TAG v20260317.182325
)

FetchContent_MakeAvailable(dawn)
