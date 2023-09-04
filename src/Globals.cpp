#include "Globals.hpp"
#include <iostream>

void Globals::InitCpu(const std::string& file) {
  if (!cpu_load_program(file.c_str(), &Globals::Instance().cpu)) {
    std::cerr << "Failed to load program [" << file << "]" << std::endl;
    return;
  }
  std::cout << "Loaded program [" << file << "]" << std::endl;

  // Disassemble cpu memory into 'instructions'
  for (int i = 0; i < 0xffff;) {
    instruction_t ins = cpu_get_instruction(i, &Globals::Instance().cpu);
    Globals::Instance().instructions.emplace(ins.address, ins);

    if (ins.bytes == 0) {
      i++;
    }
    else {
      i += ins.bytes;
    }
  }
  std::cout << "Disassembled program" << std::endl;
}
