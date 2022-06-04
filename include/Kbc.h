#pragma once
#include "IoDevice.h"
#include "common.h"

class Mouse;

class Kbc : public IoDevice {
 private:
  std::unique_ptr<Fifo<uint8_t>> fifo;
  std::unique_ptr<Object> obj;

 public:
  Kbc(Mouse& mouse);
  ~Kbc();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();
  void Push(uint8_t data);
  uint8_t Pop();
  uint8_t Front();

 private:
  Mouse* mouse = NULL;
  void ProcessCommand(uint8_t command);
  Kbc(const Kbc& other);
  void operator=(const Kbc& other);
};