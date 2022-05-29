#pragma once
#include "Fifo.h"
#include "common.h"

class IoDevice {
 protected:
  std::unique_ptr<Fifo<uint8_t>> fifo;
  std::unique_ptr<Object> obj;

 public:
  IoDevice();
  virtual ~IoDevice();
  virtual void Out8(const uint16_t addr, const uint8_t data) = 0;
  virtual uint8_t In8(const uint16_t addr) = 0;
  virtual int IsEmpty() = 0;  // TODO : IoDevice::IsEmptyの戻り値をboolに変更
  virtual void Push(
      uint8_t data);  // Mouseだけ変更するの、純粋仮想関数にしない。
  virtual uint8_t Pop();  //リアルモードで使用する
  virtual uint8_t Front();  //読み込むだけ。リアルモードで使用する
};