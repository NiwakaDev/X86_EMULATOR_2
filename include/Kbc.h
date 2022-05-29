#pragma once
#include "IoDevice.h"
#include "common.h"

class Mouse;

class Kbc : public IoDevice {
 public:
  Kbc(Mouse& mouse);
  ~Kbc();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();

 private:
  Mouse* mouse = NULL;
  void ProcessCommand(uint8_t command);
  Kbc(const Kbc& other);
  void operator=(const Kbc& other);
};