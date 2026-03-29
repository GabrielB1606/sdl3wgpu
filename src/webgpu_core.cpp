#include "webgpu_core.hpp"
#include "SDL3/SDL_video.h"
#include "webgpu/webgpu.h"

bool Core::done = false;
std::vector<SDL_Window *> Core::windows = {};
std::vector<WindowSize> Core::window_surface_size = {};
std::vector<WGPUInstance> Core::wgpu_instances = {};
std::vector<WGPUDevice> Core::wgpu_devices = {};
std::vector<WGPUSurface> Core::wgpu_surfaces = {};
std::vector<WGPUQueue> Core::wgpu_queues = {};
std::vector<WGPUSurfaceConfiguration> Core::wgpu_surface_configurations = {};

std::vector<std::vector<std::function<int(const SDL_Event)>>>
    Core::process_event_functions = {};
std::vector<std::vector<std::function<int(WGPUSurfaceTexture, const int &,
                                          const int &, void(int, int))>>>
    Core::draw_functions = {};

std::vector<std::vector<std::function<int(const WGPURenderPassEncoder)>>>
    Core::render_functions = {};
std::vector<CoreIndices> Core::configurations = {};
int Core::current_config_index = 0;

int Core::initWGPU(SDL_Window *window) {

  wgpu_instances.push_back(nullptr);
  wgpu_devices.push_back(nullptr);
  wgpu_surfaces.push_back(nullptr);
  wgpu_queues.push_back(nullptr);
  wgpu_surface_configurations.push_back({});

  return 0;
}

void Core::resizeSurface(int width, int height) {

  WGPUSurfaceConfiguration &wgpu_surface_configuration =
      wgpu_surface_configurations[configurations[current_config_index]
                                      .surface_configuration];
  WindowSize &size =
      window_surface_size[configurations[current_config_index].window];
  WGPUSurface &wgpu_surface =
      wgpu_surfaces[configurations[current_config_index].surface];

  wgpu_surface_configuration.width = size.width = width;
  wgpu_surface_configuration.height = size.height = height;
  wgpuSurfaceConfigure(wgpu_surface,
                       (WGPUSurfaceConfiguration *)&wgpu_surface_configuration);
}

int Core::init() {

  // Setup SDL
  // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts
  // would likely be your SDL_AppInit() function]
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return 1;
  }

  const int cc = windows.size();
  configurations.push_back(CoreIndices(cc));
  window_surface_size.push_back({1280, 720});

  // Create SDL window graphics context
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE;
  windows.push_back(SDL_CreateWindow(
      "Dear ImGui SDL3+WebGPU example", window_surface_size[cc].width,
      window_surface_size[cc].height, window_flags));

  if (*(windows.end() - 1) == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return 1;
  }

  process_event_functions.push_back({});

  // Initialize WGPU
  initWGPU(*(windows.end() - 1));

  return 0;
}

int Core::clean() {

  for (WGPUSurface surface : wgpu_surfaces) {
    wgpuSurfaceUnconfigure(surface);
    wgpuSurfaceRelease(surface);
  }

  for (WGPUQueue q : wgpu_queues) {
    wgpuQueueRelease(q);
  }

  for (WGPUDevice device : wgpu_devices) {
    wgpuDeviceRelease(device);
  }

  for (WGPUInstance instance : wgpu_instances) {
    wgpuInstanceRelease(instance);
  }

  for (SDL_Window *window : windows) {
    SDL_DestroyWindow(window);
  }

  SDL_Quit();

  return 0;
}

int Core::processEvents() {

  for (const CoreIndices current : configurations) {
    SDL_Window *&window = windows[current.window];
    WindowSize &window_size = window_surface_size[current.window];

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags. [If using
    // SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your
    // SDL_AppEvent() function]
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

      for (const auto f :
           process_event_functions[current.process_event_functions]) {
        const int rValue = f(event);
        if (rValue) {
          printf("process_event_functions returned with value %d\n", rValue);
        }
      }
      if (event.type == SDL_EVENT_QUIT)
        done = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
    // SDL_AppIterate() function] React to changes in screen size
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    if (width != window_size.width || height != window_size.height)
      resizeSurface(width, height);
  }

  return 0;
}

int Core::run() {

#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
  while (!done)
#endif
  {

    for (int config_index = 0; config_index < configurations.size();
         ++config_index) {

      current_config_index = config_index;
      const CoreIndices current = configurations[config_index];

      SDL_Window *&window = windows[current.window];
      WindowSize &window_size = window_surface_size[current.window];
      WGPUSurface &wgpu_surface = wgpu_surfaces[current.surface];
      WGPUSurfaceConfiguration &wgpu_surface_configuration =
          wgpu_surface_configurations[current.surface_configuration];
      WGPUDevice &wgpu_device = wgpu_devices[current.device];
      WGPUQueue &wgpu_queue = wgpu_queues[current.queue];

      processEvents();

      // Check surface status for error. If texture is not optimal, try to
      // reconfigure the surface.
      WGPUSurfaceTexture surface_texture;
      wgpuSurfaceGetCurrentTexture(wgpu_surface, &surface_texture);

      for (const auto draw : draw_functions[current.draw_functions]) {
        const int rValue = draw(surface_texture, window_size.width,
                                window_size.height, resizeSurface);
        if (rValue == -1) {
          abort();
        } else if (rValue == 1) {
          continue;
        }
      }

      WGPUTextureViewDescriptor view_desc = {};
      view_desc.format = wgpu_surface_configuration.format;
      view_desc.dimension = WGPUTextureViewDimension_2D;
      view_desc.mipLevelCount = WGPU_MIP_LEVEL_COUNT_UNDEFINED;
      view_desc.arrayLayerCount = WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
      view_desc.aspect = WGPUTextureAspect_All;

      WGPUTextureView texture_view =
          wgpuTextureCreateView(surface_texture.texture, &view_desc);

      WGPURenderPassColorAttachment color_attachments = {};
      color_attachments.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
      color_attachments.loadOp = WGPULoadOp_Clear;
      color_attachments.storeOp = WGPUStoreOp_Store;
      // const ImVec4 clear_color = DebugGUI::getClearColor();
      // color_attachments.clearValue = {
      // clear_color.x * clear_color.w, clear_color.y * clear_color.w,
      // clear_color.z * clear_color.w, clear_color.w};
      color_attachments.view = texture_view;

      WGPURenderPassDescriptor render_pass_desc = {};
      render_pass_desc.colorAttachmentCount = 1;
      render_pass_desc.colorAttachments = &color_attachments;
      render_pass_desc.depthStencilAttachment = nullptr;

      WGPUCommandEncoderDescriptor enc_desc = {};
      WGPUCommandEncoder encoder =
          wgpuDeviceCreateCommandEncoder(wgpu_device, &enc_desc);

      WGPURenderPassEncoder pass =
          wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
      for (const auto render : render_functions[current.draw_functions]) {
        render(pass);
      }
      // DebugGUI::renderDrawData(pass);
      wgpuRenderPassEncoderEnd(pass);

      WGPUCommandBufferDescriptor cmd_buffer_desc = {};
      WGPUCommandBuffer cmd_buffer =
          wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
      wgpuQueueSubmit(wgpu_queue, 1, &cmd_buffer);

#ifndef __EMSCRIPTEN__
      wgpuSurfacePresent(wgpu_surface);
      // Tick needs to be called in Dawn to display validation errors
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
      wgpuDeviceTick(wgpu_device);
#endif
#endif
      wgpuTextureViewRelease(texture_view);
      wgpuRenderPassEncoderRelease(pass);
      wgpuCommandEncoderRelease(encoder);
      wgpuCommandBufferRelease(cmd_buffer);
    }
  }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  return 0;
}
