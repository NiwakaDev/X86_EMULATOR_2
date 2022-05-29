#pragma once
#include "Fifo.h"
#include "common.h"

class IoDevice;

class Pic {
 public:
  enum PIC_REGISTERS {
    PIC0_ICW1 = 0x0020,
    PIC0_OCW2 = 0x0020,
    PIC0_IMR = 0x0021,
    PIC0_ICW2 = 0x0021,
    PIC0_ICW3 = 0x0021,
    PIC0_ICW4 = 0x0021,
    PIC1_ICW1 = 0x00a0,
    PIC1_OCW2 = 0x00a0,
    PIC1_IMR = 0x00a1,
    PIC1_ICW2 = 0x00a1,
    PIC1_ICW3 = 0x00a1,
    PIC1_ICW4 = 0x00a1,
  };
  Pic(IoDevice** io_devices);
  ~Pic();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  bool HasIrq();
  inline int GetNowIrq();

 private:
  std::unique_ptr<Object> obj;
  Pic(const Pic& other);
  void operator=(const Pic& other);
  IoDevice** io_devices = NULL;
  bool irq_list[16];  //割り込み許可
  int now_irq_num;
};

#include "detail/Pic.h"