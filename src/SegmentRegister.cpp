#include "SegmentRegister.h"
#include "Cpu.h"
using namespace std;

SegmentRegister::SegmentRegister(uint16_t data){
    this->selector.raw = data;
    this->cache.base_addr = 0x0;
    this->cache.limit     = 0x0000FFFF;
    this->cache.bit32_mode = false;//リアルモードでは16ビットモードがデフォルトで、コードセグメントによる変更はできない。
}

bool SegmentRegister::Is32bitsMode(){
    return this->cache.bit32_mode;
}

void SegmentRegister::On32bitMode(){
    this->cache.bit32_mode = true;
}

void SegmentRegister::SetCache(Cpu* cpu, uint16_t idx){
    GdtGate* gdt_gate;
    if(this->selector.ti){//LDTへのアクセス
        gdt_gate  = cpu->GetLdtGate(idx);
    }else{
        gdt_gate  = cpu->GetGdtGate(idx);
    }
    this->cache.base_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;
    this->cache.limit     = 0x03FFFFFF & ((((uint32_t)gdt_gate->limit_high)<<16) | (uint32_t)gdt_gate->limit_low);
    this->cache.bit32_mode = (gdt_gate->limit_high&DB)==DB;
    this->dpl = GET_DPL(gdt_gate->access_right);
    this->cpl             = this->dpl;
}

void SegmentRegister::Set(uint16_t data, Cpu* cpu){
    if(cpu->IsProtectedMode()){
        this->selector.raw = data;
        this->SetCache(cpu, this->selector.idx<<3);
        return;
    }
    this->selector.raw = data;
}

uint8_t SegmentRegister::GetDpl(){
    return this->cpl;
}

void SegmentRegister::SetRpl(uint8_t rpl){
    this->selector.rpl = rpl;
}

void SegmentRegister::SetCpl(uint8_t rpl){
    this->cpl = rpl;
}