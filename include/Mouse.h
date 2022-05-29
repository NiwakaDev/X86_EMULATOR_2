#pragma once
#include "Fifo.h"
#include "IoDevice.h"
#include "common.h"

class Pic;

class Mouse : public IoDevice {
 public:
  Mouse();
  ~Mouse();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();
  void SetEnable();
  inline void Push(uint8_t data);

 private:
  Mouse(const Mouse& other);
  void operator=(const Mouse& other);
  bool enable_flg;
};

#include "detail/Mouse.h"