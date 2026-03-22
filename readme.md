# WebGPU cross-platform app with CMake/Emscripten (definetly not a copy of [this](https://github.com/beaufortfrancois/webgpu-cross-platform-app/blob/main/README.md?plain=1) with [imgui](https://github.com/ocornut/imgui) and [SDL3](https://github.com/libsdl-org/SDL))

This app is a <em>"minimalistic"</em> C++ example that shows how to use [WebGPU](https://gpuweb.github.io/gpuweb/) to build desktop and web apps from a single codebase. Under the hood, it uses WebGPU's [webgpu.h](https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h) as a stable and platform-agnostic hardware abstraction layer through a C++ wrapper called [webgpu_cpp.h](https://source.chromium.org/chromium/chromium/src/+/main:third_party/dawn/include/webgpu/webgpu_cpp.h). Note that this wrapper is subject to change.

On the web (not tested yet lol), the app is built against [Emdawnwebgpu](https://dawn.googlesource.com/dawn/+/refs/heads/main/src/emdawnwebgpu/) (Emscripten Dawn WebGPU), which has bindings implementing webgpu.h on top of the JavaScript API. It uses [Emscripten](https://emscripten.org/), a tool for compiling C/C++ programs to WebAssembly. On specific platforms such as macOS or Windows, this project can be built against [Dawn](https://dawn.googlesource.com/dawn/), Chromium's cross-platform WebGPU implementation. While webgpu.h is considered stable, this stability doesn't include extensions added by Emdawnwebgpu or Dawn.

## Setup

```sh
# Clone repository and initialize submodules.
git clone https://github.com/GabrielB1606/sdl3wgpu
cd sdl3wgpu/
git submodule update --init
```

## Requirements
- clang (recommended, wgpu might behave funny with g++)
- cmake
- enscripten (not necessary for native compilation)
- one of these APIs:
    - Vulkan
    - DirectX 12 (any version should work tho, but it has only been tested in 12)
    - Metal (not tested)

## Build

```sh
# Build the app with CMake.
cmake -B build
cmake --build build -j4

# Run the app (you can change the program name in the src CMakeLists.txt)
./build/bin/myapp
```

## Web build (WIP)

```sh
# Build the app with Emscripten.
emcmake cmake -B build-web
cmake --build build-web -j4

# Run a server.
npx http-server
```

```sh
# Open the web app.
open http://127.0.0.1:8080/build-web/app.html
```

### Debugging WebAssembly

When building the app, compile it with DWARF debug information included thanks to `emcmake cmake -DCMAKE_BUILD_TYPE=Debug -B build-web`. And make sure to install the [C/C++ DevTools Support (DWARF) Chrome extension](https://goo.gle/wasm-debugging-extension) to enable WebAssembly debugging in DevTools.

<img width="1112" alt="image" src="https://github.com/beaufortfrancois/webgpu-cross-platform-app/assets/634478/e82f2494-6b1a-4534-b9e3-0c04caeca96d">
