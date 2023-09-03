#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui-src/imgui.h>

#include <iostream>
#include "ImGuiLayer.hpp"
#include "WindowUtil.hpp"

int main() {
  if (!WindowUtil::GLFW_Init()) {
    return 1;
  }

  GLFWwindow* window = WindowUtil::CreateWindow("6502 Emu", 600, 600);
  if (!window) {
    return 2;
  }

  ImGuiLayer::Init(window);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGuiLayer::BeginFrame();

    ImGui::Begin("Window");

    ImGui::End();

    ImGuiLayer::EndFrame();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGuiLayer::Deinit();
  WindowUtil::GLFW_Deinit(window);
}
