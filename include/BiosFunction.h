#pragma once
#include "Fifo.h"
#include "common.h"
class Cpu;
class Memory;
class Vga;
class Kbc;

class BiosFunction {
 protected:
  std::unique_ptr<Object> obj;
  std::string function_name;

 public:
  BiosFunction();
  virtual ~BiosFunction();
  virtual void Run(Cpu& cpu, Memory& mem) = 0;
};

class VideoFunction : public BiosFunction {
 private:
  uint8_t color_table[8] = {30, 34, 32, 36, 31, 35, 33, 37};
  void out8(uint16_t addr, uint8_t value);
  void ClearConsole();
  Vga* vga;
  uint8_t console_buff[25]
                      [80];  // TODO:動的に配列の内容を変更できるようにする。
 public:
  VideoFunction(Vga& vga);
  ~VideoFunction();
  void Run(Cpu& cpu, Memory& mem);
};

class FloppyFunction : public BiosFunction {
 private:
  const int32_t floppy_size = 1474560;
  std::unique_ptr<uint8_t[]> buff;
  void Read(Cpu& cpu, const Memory& mem);

 public:
  void Run(Cpu& cpu, Memory& mem);
  FloppyFunction(std::function<void(uint8_t* buff, int size)> read_callback);
};

class MemoryFunction : public BiosFunction {
 private:
  void out8(uint16_t addr, uint8_t value);

 public:
  MemoryFunction();
  ~MemoryFunction();
  void Run(Cpu& cpu, Memory& mem);
};

class EquipmentListFunction : public BiosFunction {
 private:
  void out8(uint16_t addr, uint8_t value);

 public:
  EquipmentListFunction();
  ~EquipmentListFunction();
  void Run(Cpu& cpu, Memory& mem);
};

class KeyFunction : public BiosFunction {
 private:
  Kbc* kbc;

 public:
  KeyFunction(Kbc& kbc);
  ~KeyFunction();
  void Run(Cpu& cpu, Memory& mem);
  void In();
  uint16_t Decode(uint16_t scan_code);
};

class TimerFunction : public BiosFunction {
 public:
  TimerFunction();
  ~TimerFunction();
  void Run(Cpu& cpu, Memory& mem);
};

class GeneralSystemServicesFunction : public BiosFunction {
 public:
  GeneralSystemServicesFunction();
  ~GeneralSystemServicesFunction();
  void Run(Cpu& cpu, Memory& mem);
};