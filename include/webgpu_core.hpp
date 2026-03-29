#ifndef WEBGPU_CORE_HPP
#define WEBGPU_CORE_HPP

#include <SDL3/SDL.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// This example can also compile and run with Emscripten! See
// 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#include <webgpu/webgpu_cpp.h>
#endif

struct CoreIndices {
  int instance, device, surface, queue, surface_configuration,
      process_event_functions, window, draw_functions;

  CoreIndices(int value)
      : instance(value), device(value), surface(value), queue(value),
        surface_configuration(value), process_event_functions(value),
        window(value), draw_functions(value) {}
};

struct WindowSize {
  int width, height;
};

class Core {

  static std::vector<SDL_Window *> windows;
  static std::vector<WindowSize> window_surface_size;

  static std::vector<WGPUInstance> wgpu_instances;
  static std::vector<WGPUDevice> wgpu_devices;
  static std::vector<WGPUSurface> wgpu_surfaces;
  static std::vector<WGPUQueue> wgpu_queues;
  static std::vector<WGPUSurfaceConfiguration> wgpu_surface_configurations;

  static std::vector<std::vector<std::function<int(const SDL_Event)>>>
      process_event_functions;
  static std::vector<std::vector<std::function<int(
      WGPUSurfaceTexture, const int &, const int &, void(int, int))>>>
      draw_functions;

  static std::vector<
      std::vector<std::function<int(const WGPURenderPassEncoder)>>>
      render_functions;
  static bool done;

  static int initWGPU(SDL_Window *window);

  static std::vector<CoreIndices> configurations;

  static int current_config_index;

public:
  static int init();
  WGPUSurface createWGPUSurface(const WGPUInstance &instance,
                                SDL_Window *window);
  static void resizeSurface(int width, int height);
  static int clean();

  static void stop() { done = false; }
  static bool isRunning() { return !done; }

  static int run();
  static int processEvents();
};

#endif
