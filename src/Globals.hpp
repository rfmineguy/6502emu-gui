#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <6502emu/cpu.h>
#include <6502emu/dbginfo.h>
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
  cc65_dbginfo dbg_info;
  std::map<uint16_t, instruction_t> instructions;
};

#endif
