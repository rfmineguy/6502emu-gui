#include "Globals.hpp"
#include <iostream>

void cc65_err_callback(const cc65_parseerror* error) {
  std::cerr << "cc65_err_callback" << std::endl;
}

void Globals::InitCpu(const std::string& file) {
  if (!cpu_load_program(file.c_str(), &Globals::Instance().cpu)) {
    std::cerr << "Failed to load program [" << file << "]" << std::endl;
    return;
  }
  std::cout << "Loaded program [" << file << "]" << std::endl;

  // Disassemble cpu memory into 'instructions'
  Globals::Instance().instructions.clear();
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

  // Load debug file
  std::string debug_filename = file;
  debug_filename += ".dbg";
  Globals::Instance().dbg_info = cc65_read_dbginfo(debug_filename.c_str(), &cc65_err_callback);
  std::cout << "Parsed : " << debug_filename << std::endl;
}
