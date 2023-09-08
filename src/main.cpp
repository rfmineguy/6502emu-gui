#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui-src/imgui.h>

#include "ImGuiLayer.hpp"
#include "WindowUtil.hpp"
#include "Globals.hpp"

int main() {
  if (!WindowUtil::GLFW_Init()) {
    return 1;
  }

  GLFWwindow* window = WindowUtil::CreateWindow("6502 Emu", 600, 600);
  if (!window) {
    return 2;
  }

  ImGuiLayer::Init(window);
  // Globals::InitCpu("file");

  while (!glfwWindowShouldClose(window)) {
    ImGuiLayer::BeginFrame();
    ImGuiLayer::BeginDockspace();
    ImGuiLayer::ShowMenuBar();

    ImGuiLayer::ShowRegisters();
    ImGuiLayer::ShowWatch();
    ImGuiLayer::ShowCode();
    ImGuiLayer::ShowControls();
    ImGuiLayer::ShowMemEdit();

    ImGuiLayer::ShowDebugInfo();

    ImGui::ShowDemoWindow();

    ImGuiLayer::EndDockspace();
    ImGuiLayer::EndFrame();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGuiLayer::Deinit();
  WindowUtil::GLFW_Deinit(window);
}
