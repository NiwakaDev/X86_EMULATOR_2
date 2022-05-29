#include "Pic.h"
#include "IoDevice.h"
using namespace std;

Pic::Pic(IoDevice** io_devices) {
  if (io_devices == NULL) {
    this->obj->Error("io_devices is NULL at Pic::Pic");
  }
  this->obj = make_unique<Object>();
  for (int i = 0; i < 16; i++) {
    this->irq_list[i] = false;
  }
  this->io_devices = io_devices;
}

Pic::~Pic() {}

void Pic::Out8(const uint16_t addr, const uint8_t data) {
  switch (addr) {
    case PIC1_ICW1:
    case PIC0_ICW1:
      break;
    case PIC0_IMR:
      for (int i = 0; i < 8; i++) {
        this->irq_list[i] = !((data & (1 << i)) == (1 << i));
      }
      break;
    case PIC1_IMR:
      for (int i = 8; i < 16; i++) {
        this->irq_list[i] =
            !(((((unsigned int)data) << 8) & (1 << i)) == (1 << i));
      }
      break;
    default:
      this->obj->Error("Not implemented: io_addr=%04X\n", addr);
  }
}

uint8_t Pic::In8(const uint16_t addr) {
  switch (addr) {
    default:
      this->obj->Error("Not implemented addr=0x%04X at Pic::In8", addr);
  }
}

bool Pic::HasIrq() {
  for (int i = 0; i < 16; i++) {
    if (this->io_devices[i] == NULL) {
      continue;
    }
    if (!this->irq_list[i]) {
      continue;
    }
    // TODO : 入出力デバイスが割り込み番号を知っていない実装に変更
    if ((this->now_irq_num = this->io_devices[i]->IsEmpty()) == -1) {
      continue;
    }
    return true;
  }
  return false;
}