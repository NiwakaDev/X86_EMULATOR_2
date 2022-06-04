#include "Mouse.h"
#include "Pic.h"
using namespace std;

Mouse::Mouse() {
  this->fifo = make_unique<Fifo<uint8_t>>();
  this->obj = make_unique<Object>();
  this->enable_flg = false;
}

Mouse::~Mouse() {}

void Mouse::Out8(const uint16_t addr, const uint8_t data) {
  this->obj->Error("Not implmented: Mouse::Out8");
}

uint8_t Mouse::In8(const uint16_t addr) {
  switch (addr) {
    case 0x60:
      return this->fifo->Pop();
    default:
      this->obj->Error("Not implemented: addr=0x%02X at Mouse::In8", addr);
  }
}

int Mouse::IsEmpty() { return this->fifo->IsEmpty() ? -1 : 0x0C; }

uint8_t Mouse::Pop() { return this->fifo->Pop(); }

uint8_t Mouse::Front() {
  throw runtime_error("Not implemented: Mouse::Front");
}