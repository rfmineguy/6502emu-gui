#include "WindowUtil.hpp"
#include <glad/glad.h>
#include <iostream>

namespace WindowUtil {
  int GLFW_Init() {
    if (!glfwInit()) {
      std::cerr << "Failed to initialize glfw" << std::endl;
      return 0;
    }
    return 1;
  }

  void GLFW_Deinit(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  GLFWwindow* CreateWindow(const std::string& title, const int width, const int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "6502 Emu", NULL, NULL);
    if (!window) {
      std::cerr << "Failed to initialize glfw window" << std::endl;
      return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize glad" << std::endl;
      return nullptr;
    }
    glViewport(0, 0, 640, 480);

    return window;
  }

  void DestroyWindow(GLFWwindow* wPtr) {
    
  }

  void Loop(GLFWwindow*, std::function<void()>);
}
