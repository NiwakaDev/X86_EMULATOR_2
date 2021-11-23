#pragma once
#include "common.h"

typedef struct _Cache{
    uint32_t base_addr;
    uint32_t limit;
    bool bit32_mode;
}Cache;

class Cpu;

class SegmentRegister:public Object{
    protected:
        Cache cache;
        void SetCache(Cpu* cpu, uint16_t idx);
        union{
            uint16_t raw;
            struct{
                unsigned rpl :2;
                unsigned ti  :1;//0=GDT, 1=LDT
                unsigned idx :13;
            };
        }selector;
        uint8_t dpl;
        uint8_t cpl;
    public:
        SegmentRegister(uint16_t data);
        uint32_t GetBaseAddr();
        uint32_t GetLimit();
        bool Is32bitsMode();
        uint16_t GetData();
        uint8_t GetDpl();
        void SetRpl(uint8_t rpl);
        void SetCpl(uint8_t cpl);
        void Set(uint16_t data, Cpu* cpu);
        void On32bitMode();
};