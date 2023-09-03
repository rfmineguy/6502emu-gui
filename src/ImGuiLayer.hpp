#ifndef IMGUI_LAYER
#define IMGUI_LAYER
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace ImGuiLayer {
  void Init(GLFWwindow*);
  void Deinit();
  void BeginFrame();
  void EndFrame();
}

#endif
