
#include "Cpu.h"
#include "Ldtr.h"

Ldtr::Ldtr(uint16_t data):SegmentRegister(data){
    this->selector.raw = data;
    this->cache.base_addr = 0x0;
    this->cache.limit     = 0x0000FFFF;
}

void Ldtr::SetCache(Cpu* cpu, uint16_t selector){
    GdtGate* gdt_gate  = cpu->GetGdtGate(selector);
    this->cache.base_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;
    this->cache.limit     = 0x03FFFFFF & ((((uint32_t)gdt_gate->limit_high)<<16) | (uint32_t)gdt_gate->limit_low);
    this->cache.bit32_mode = (gdt_gate->limit_high&DB)==DB;
    if((gdt_gate->access_right&0x1D)==LDT_TYPE){
        return;
    }
    this->Error("This gdt_gate is not Ldt at Ldtr::SetCache");
}