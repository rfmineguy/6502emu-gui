cmake_minimum_required(VERSION 3.20)
project(6502emu_gui LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glad GIT_REPOSITORY https://github.com/rfmineguy/glad-rf.git)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 10.1.1)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(6502emu GIT_REPOSITORY https://github.com/rfmineguy/6502emu.git)
FetchContent_MakeAvailable(6502emu)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        docking
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
  file(COPY cmake/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  include_directories(${imgui_SOURCE_DIR}/include/ ${glfw_SOURCE_DIR}/include/)
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
endif()

add_executable(6502emu_gui src/main.cpp src/ImGuiLayer.cpp src/WindowUtil.cpp src/Globals.cpp)
set_property(TARGET 6502emu_gui PROPERTY CXX_STANDARD 17)
target_link_libraries(6502emu_gui PRIVATE glfw imgui glad fmt 6502lib)
target_include_directories(6502emu_gui PRIVATE ${glad_SOURCE_DIR}/include ${glfw_SOURCE_DIR}/include/ ${imgui_SOURCE_DIR}/include ${fmt_SOURCE_DIR}/include/ ${6502emu_SOURCE_DIR}/include)
