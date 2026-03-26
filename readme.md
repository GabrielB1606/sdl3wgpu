# WebGPU cross-platform app with CMake/Emscripten (definetly not a copy of [this](https://github.com/beaufortfrancois/webgpu-cross-platform-app/blob/main/README.md?plain=1) with [imgui](https://github.com/ocornut/imgui) and [SDL3](https://github.com/libsdl-org/SDL))

This app is a <em>"minimalistic"</em> C++ example that shows how to use [WebGPU](https://gpuweb.github.io/gpuweb/) to build desktop and web apps from a single codebase. Under the hood, it uses WebGPU's [webgpu.h](https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h) as a stable and platform-agnostic hardware abstraction layer through a C++ wrapper called [webgpu_cpp.h](https://source.chromium.org/chromium/chromium/src/+/main:third_party/dawn/include/webgpu/webgpu_cpp.h). Note that this wrapper is subject to change.

On the web, the app is built against [Emdawnwebgpu](https://dawn.googlesource.com/dawn/+/refs/heads/main/src/emdawnwebgpu/) (Emscripten Dawn WebGPU), via Emscripten’s `--use-port=emdawnwebgpu`, which implements `webgpu.h` on top of the browser WebGPU API. It uses [Emscripten](https://emscripten.org/) to compile C++ to WebAssembly. On desktop, this project builds against [Dawn](https://dawn.googlesource.com/dawn/) from `vendors/dawn`. While `webgpu.h` is considered stable, that does not cover extensions added by Emdawnwebgpu or Dawn.

Upstream reference for the UI stack: [imgui `example_sdl3_wgpu`](https://github.com/ocornut/imgui/tree/master/examples/example_sdl3_wgpu).

## Setup

```sh
# Clone repository and initialize submodules (Dawn is required for native builds only).
git clone https://github.com/GabrielB1606/sdl3wgpu
cd sdl3wgpu/
git submodule update --init
```

## Requirements

- **Native:** CMake ≥ 3.22, a C++17 compiler (Clang recommended), and a supported GPU backend (Vulkan, D3D12, or Metal).
- **Web (Emscripten):** [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) **≥ 4.0.15** (required for SDL3 + WebGPU in this setup, same constraint as imgui’s SDL3 example).
- **Build tool:** [Ninja](https://ninja-build.org/) is recommended (`-G Ninja`). `Unix Makefiles` works if Ninja is not installed.

## Native build

```sh
cmake -B build
cmake --build build -j4

# Run (binary name is `myapp` unless you change it in `src/CMakeLists.txt`)
./build/bin/myapp
```

## Emscripten (web) build

Configure with **`emcmake`** so CMake uses the Emscripten toolchain. Use a **separate build directory** from native builds.

```sh
# From the repository root, with Emscripten on PATH (e.g. after `source emsdk_env.sh`).
emcmake cmake -G Ninja -B build-em
cmake --build build-em -j4
```

Without Ninja:

```sh
emcmake cmake -G "Unix Makefiles" -B build-em
cmake --build build-em -j4
```

### What gets produced

- The HTML/JS/WASM output is written under **`build-em/bin/`**.
- The main page is **`index.html`** (CMake sets `OUTPUT_NAME` to `index` and `CMAKE_EXECUTABLE_SUFFIX` to `.html`).
- Keep **`index.html`**, **`index.js`**, and **`index.wasm`** (and any other emitted files) in the same directory when serving.

### Run in the browser

You must serve the **`build-em/bin`** folder over **HTTP** (browsers typically block loading `.wasm` from `file://`).

```sh
emrun --no_browser --port 3000 build-em/bin/index.html
```

### Debug build (WebAssembly)

```sh
emcmake cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -B build-em
cmake --build build-em -j4
```

For stepping through C++ in Chrome, install the [C/C++ DevTools Support (DWARF) Chrome extension](https://goo.gle/wasm-debugging-extension).

<img width="1112" alt="image" src="https://github.com/beaufortfrancois/webgpu-cross-platform-app/assets/634478/e82f2494-6b1a-4534-b9e3-0c04caeca96d">
