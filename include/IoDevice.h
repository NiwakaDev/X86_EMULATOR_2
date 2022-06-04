#pragma once
#include "Fifo.h"
#include "common.h"

class IoDevice {
 public:
  virtual ~IoDevice(){};
  virtual void Out8(const uint16_t addr, const uint8_t data) = 0;
  virtual uint8_t In8(const uint16_t addr) = 0;
  virtual int IsEmpty() = 0;  // TODO : IoDevice::IsEmptyの戻り値をboolに変更
  virtual void Push(
      uint8_t data) = 0;  
  virtual uint8_t Pop() = 0; 
  virtual uint8_t Front() = 0;  
};