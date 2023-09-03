#ifndef WINDOW_UTIL
#define WINDOW_UTIL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace WindowUtil {
  int GLFW_Init();
  void GLFW_Deinit(GLFWwindow*);

  GLFWwindow* CreateWindow(const std::string&, const int, const int);
  void DestroyWindow(GLFWwindow*);

  void Loop(GLFWwindow*, std::function<void()>);
}

#endif
