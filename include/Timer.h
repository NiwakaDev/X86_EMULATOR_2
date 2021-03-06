#pragma once
#include "Fifo.h"
#include "IoDevice.h"
#include "common.h"

class Pic;

class Timer : public IoDevice {
 public:
  Timer();
  ~Timer();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();
  void Push(uint8_t data);
  uint8_t Pop();
  uint8_t Front();

 private:
  Timer(const Timer& other);
  void operator=(const Timer& other);
  bool enable = false;
  std::unique_ptr<std::thread> timer_thread;
  void Run(uint32_t cycle);
  std::unique_ptr<Fifo<uint8_t>> fifo;
  std::unique_ptr<Object> obj;
};