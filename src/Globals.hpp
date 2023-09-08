#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <6502emu/cpu.h>
#include <map>

class Globals {
public:
  static Globals& Instance() {
    static Globals g;
    return g;
  }

public:
  Globals() {}
  ~Globals() {}
  static void InitCpu(const std::string&);

public:
  cpu_t cpu;
  std::map<uint16_t, instruction_t> instructions;
};

#endif
