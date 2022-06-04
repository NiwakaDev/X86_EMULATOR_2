#pragma once
#include "Fifo.h"
#include "IoDevice.h"
#include "common.h"

class Pic;

class Mouse : public IoDevice {
 private:
  std::unique_ptr<Fifo<uint8_t>> fifo;
  std::unique_ptr<Object> obj;

 public:
  Mouse();
  ~Mouse();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();
  void SetEnable();
  inline void Push(uint8_t data);
  uint8_t Pop();
  uint8_t Front();

 private:
  Mouse(const Mouse& other);
  void operator=(const Mouse& other);
  bool enable_flg;
};

#include "detail/Mouse.h"