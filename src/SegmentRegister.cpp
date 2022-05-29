#include "SegmentRegister.h"
#include "Cpu.h"
using namespace std;

SegmentRegister::SegmentRegister(uint16_t data) {
  this->obj = make_unique<Object>();
  this->selector.raw = data;
  this->cache.base_addr = 0x0;
  this->cache.limit = 0x0000FFFF;
  this->cache.bit32_mode =
      false;  //リアルモードでは16ビットモードがデフォルトで、コードセグメントによる変更はできない。
}

bool SegmentRegister::Is32bitsMode() { return this->cache.bit32_mode; }

void SegmentRegister::On32bitMode() { this->cache.bit32_mode = true; }

void SegmentRegister::SetCache(const GdtGate& gdt_gate) {
  this->cache.base_addr = (((uint32_t)gdt_gate.base_high) << 24) |
                          (((uint32_t)gdt_gate.base_mid) << 16) |
                          (uint32_t)gdt_gate.base_low;
  this->cache.limit = 0x000FFFFF & ((((uint32_t)gdt_gate.limit_high) << 16) |
                                    (uint32_t)gdt_gate.limit_low);
  this->cache.bit32_mode = (gdt_gate.limit_high & DB) == DB;
  this->dpl = CpuHelper::GetDpl(gdt_gate.access_right);
  this->cpl = this->dpl;
  this->selector.rpl = this->dpl;
}

void SegmentRegister::Set(const uint16_t data,
                          function<GdtGate*(uint16_t)> GetGdtGate,
                          function<GdtGate*(uint16_t)> GetLdtGate,
                          bool protected_mode) {
  if (protected_mode) {
    this->selector.raw = data;
    GdtGate* gdt_gate;
    if (this->selector.ti) {  // LDTへのアクセス
      gdt_gate = GetLdtGate(this->selector.idx << 3);
    } else {
      gdt_gate = GetGdtGate(this->selector.idx << 3);
    }
    if (data != 0) this->SetCache(*gdt_gate);
    return;
  }
  this->selector.raw = data;
  if (data != 0) this->cache.base_addr = data * 16;
}

uint8_t SegmentRegister::GetDpl() { return this->cpl; }

void SegmentRegister::SetRpl(uint8_t rpl) { this->selector.rpl = rpl; }

void SegmentRegister::SetCpl(uint8_t rpl) { this->cpl = rpl; }