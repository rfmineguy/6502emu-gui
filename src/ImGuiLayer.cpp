#include "ImGuiLayer.hpp"
#include <imgui-src/imgui.h>
#include <imgui-src/backends/imgui_impl_glfw.h>
#include <imgui-src/backends/imgui_impl_opengl3.h>
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <nfd-src/src/include/nfd.hpp>

#include <6502emu/cpu.h>
#include "Globals.hpp"

namespace ImGuiLayer {
  void Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
  }

  void Deinit() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void BeginFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();

    // Enable docking for the next frame
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::NewFrame();
  }

  void EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void BeginDockspace() {
    static bool opt_fullscreen = true;
    static bool opt_is_open = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
      const ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus; 
    }
    else {
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;
    
    if (!opt_padding) {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    }
    ImGui::Begin("Dockspace Begin", &opt_is_open, window_flags);
    if (!opt_padding) {
      ImGui::PopStyleVar();
    }
    if (opt_fullscreen) {
      ImGui::PopStyleVar(2);
    }
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("DockspaceID");
      ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
    }
    else {
      // Docking is disabled
      // NOTE: Not sure how to recover or what to do here
    }
  }

  void EndDockspace() {
    ImGui::End();
  }

  void ShowMenuBar() {
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load Program")) {
          NFD::UniquePath path;
          nfdresult_t result = NFD::OpenDialog(path);
          if (result == NFD_OKAY) {
            std::cout << "[NFD] Open file: " << path.get() << std::endl;
            try {
              // project.Create(path.get());
            } catch (std::string& err) {
              std::cerr << "[NFD] Err: " << err << std::endl;
            }
          }
          else if (result == NFD_CANCEL) {
            std::cerr << "[NFD] Canceled" << std::endl;
          }
          else {
            std::cerr << "[NFD] " << NFD_GetError() << std::endl;
          }
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }

  // Actual UI Rendering
  void ShowRegisters() {
    ImGui::Begin("Registers");
      ImGui::Text("   A: %d", Globals::Instance().cpu.regA);
      ImGui::Text("   X: %d", Globals::Instance().cpu.regX);
      ImGui::Text("   Y: %d", Globals::Instance().cpu.regY);
      ImGui::Separator();
      ImGui::Text("   Status: 0000-000");
      ImGui::Text("   N: %d", 0);
    ImGui::End();
  }
  
  void ShowWatch() {
    ImGui::Begin("Watch");
      static std::unordered_map<uint16_t, std::string> watched_addresses;

      static char address_string[5] = {0};
      bool is_valid = false;
      ImGui::InputText("addr", address_string, 5, ImGuiInputTextFlags_CharsHexadecimal);
      ImGui::SameLine();
      int value;
      std::stringstream ss;
      ss << std::hex << address_string;
      ss >> value;

      if (value >= 0x0 && value <= 0xffff && address_string[0] != 0) {
        is_valid = true;
      }

      if (ImGui::Button(is_valid ? "T" : "F") && is_valid) {
        watched_addresses.emplace(value, "--");
      }

      uint16_t address_to_remove;
      bool     should_remove;
      for (auto& [key, value] : watched_addresses) {
        ImGui::Text("%4X  : %s", key, value.c_str());
        ImGui::SameLine();
        if (ImGui::Button("X")) {
          address_to_remove = key;
          should_remove     = true;
        }
      }
      if (should_remove) {
        auto it = watched_addresses.find(address_to_remove);
        if (it != watched_addresses.end())
          watched_addresses.erase(it);
      }
    ImGui::End();
  }
  
  void ShowCode() {
    ImGui::Begin("Code");

    ImGui::End();
  }

  void ShowControls() {
    ImGui::Begin("Controls");

    ImGui::End();
  }
}
