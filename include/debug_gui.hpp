#ifndef DEBUG_GUI_HPP
#define DEBUG_GUI_HPP

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_wgpu.h"
#include <stdio.h>
#include <stdlib.h>

class DebugGUI {

  static ImGuiIO *io;
  static ImGuiStyle *style;
  static bool show_demo_window;
  static bool show_another_window;
  static ImVec4 clear_color;

public:
  static ImVec4 getClearColor() { return clear_color; }

  static int init(SDL_Window *window, const float &main_scale,
                  const WGPUDevice &wgpu_device,
                  const WGPUSurfaceConfiguration &wgpu_surface_configuration);

  static int processEvents(const SDL_Event &event);

  static int draw(WGPUSurfaceTexture surface_texture, const int &width,
                  const int &height, void (*ResizeSurface)(int, int));

  static int renderDrawData(const WGPURenderPassEncoder &pass);

  static int clean();
};

#endif
