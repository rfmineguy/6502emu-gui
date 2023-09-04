#include "Globals.hpp"
#include <iostream>

void Globals::InitCpu(const std::string& file) {
  if (!cpu_load_program(file.c_str(), &Globals::Instance().cpu)) {
    std::cerr << "Failed to load program [" << file << "]" << std::endl;
    return;
  }
}
