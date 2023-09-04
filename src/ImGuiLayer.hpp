#ifndef IMGUI_LAYER
#define IMGUI_LAYER
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace ImGuiLayer {
  void Init(GLFWwindow*);
  void Deinit();
  void BeginFrame();
  void EndFrame();
  void BeginDockspace();
  void EndDockspace();
  void ShowMenuBar();

  // Acutal UI Rendering
  void ShowRegisters();
  void ShowWatch();
  void ShowCode();
  void ShowControls();
}

#endif
