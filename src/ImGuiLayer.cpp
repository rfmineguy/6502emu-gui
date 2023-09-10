#include "ImGuiLayer.hpp"
#include <imgui-src/imgui.h>
#include <imgui-src/backends/imgui_impl_glfw.h>
#include <imgui-src/backends/imgui_impl_opengl3.h>
#include <imgui_memory_editor.h>
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
            Globals::InitCpu(path.get());
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
      ImGui::Text("   A: %02X", Globals::Instance().cpu.regA);
      ImGui::Text("   X: %02X", Globals::Instance().cpu.regX);
      ImGui::Text("   Y: %02X", Globals::Instance().cpu.regY);
      ImGui::Separator();
      ImGui::Text("  PC: %04X", Globals::Instance().cpu.pc);
      ImGui::Separator();
      ImGui::Text("   Status: 0000-000");
      ImGui::Text("   N: %d", 0);
    ImGui::End();
  }
  
  void ShowWatch() {
    ImGui::Begin("Watch");
      static std::unordered_map<uint16_t, uint8_t*> watched_addresses;

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
        watched_addresses.emplace(value, &Globals::Instance().cpu.memory[value]);
      }

      uint16_t address_to_remove;
      bool     should_remove;
      for (auto& [key, value] : watched_addresses) {
        ImGui::Text("%4X  : %2X", key, *value);
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
    if (!Globals::Instance().cpu.loaded) {
      ImGui::Text("Program not loaded");
      ImGui::End(); 
      return;
    } 

    static char hex[5];
    if (ImGui::InputText("addr_input", hex, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
      std::cout << "Submit" << std::endl;
    }
    ImGui::Separator();

    ImGuiListClipper clipper;
    clipper.Begin(Globals::Instance().instructions.size());

    while (clipper.Step()) {
      auto it = Globals::Instance().instructions.begin();
      for (int i = 0; i < clipper.DisplayStart; i++) {
        it ++;
      }
      for (int line = clipper.DisplayStart; line < clipper.DisplayEnd; line++) {
        // whether the address is in a valid segment range
        if (it != Globals::Instance().instructions.end()) {
          instruction_t ins = it->second;

          ImGui::PushID(line);
          ImGui::Checkbox("", (bool*) &it->second.is_bp);
          ImGui::PopID();

          if (ins.address == Globals::Instance().cpu.pc)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 40, 40, 255));
          ImGui::SameLine(); ImGui::Text("%04X", it->first);
          ImGui::SameLine(); ImGui::Text("%s", ins.str);
          if (ins.address == Globals::Instance().cpu.pc)
            ImGui::PopStyleColor();
        } else { }
        it ++;
      }
    }
    clipper.End();

    // for (int i = 0; i < 0xffff; i++) {
    //   try {
    //     instruction_t ins = Globals::Instance().instructions.at(i);
    //     ImGui::Text("%02X%02X", ins.address & 0xff, ins.address & 0xff00);
    //   } catch (...) {
    //     ImGui::Text("");
    //   }
    // }
    ImGui::End();
  }

  void ShowControls() {
    ImGui::Begin("Controls");
    if (!Globals::Instance().cpu.loaded) {
      ImGui::Text("Program not loaded");
      ImGui::End();
      return;
    }

    if (ImGui::Button("Reset")) {
      cpu_reset(&Globals::Instance().cpu);
    }
    if (ImGui::Button("Step 1")) {
      instruction_t ins = cpu_step(&Globals::Instance().cpu);
      cpu_execute(&Globals::Instance().cpu, ins);
    }

    if (ImGui::Button("Continue")) {
      // ...
    }

    ImGui::End();
  }

  void ShowMemEdit() {
    static MemoryEditor mem;
    mem.DrawWindow("Memory Editor", Globals::Instance().cpu.memory, 0xffff);
  }

  void ShowDebugInfo() {
    ImGui::Begin("Debug");
    ImGui::Text("%0.4f", 1.0f / ImGui::GetIO().DeltaTime);
    ImGui::End();
  }
}
