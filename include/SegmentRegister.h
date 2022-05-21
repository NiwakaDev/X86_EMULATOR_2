#pragma once
#include "common.h"

typedef struct _Cache{
    uint32_t base_addr;
    uint32_t limit;
    bool bit32_mode;
}Cache;

struct _GdtGate;

class SegmentRegister{
    private:
        void SetCache(const struct _GdtGate& gdt_gate, const uint16_t idx);
    //TODO : 継承を廃止にし、protectedを削除する
    protected:
        std::unique_ptr<Object> obj;
        Cache cache;
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
        uint32_t GetBaseAddr(){
            return this->cache.base_addr;
        }
        uint32_t GetLimit(){
            return this->cache.limit;
        }
        uint16_t GetData(){
            return this->selector.raw;
        }
        bool Is32bitsMode();
        uint8_t GetDpl();
        void SetRpl(uint8_t rpl);
        void SetCpl(uint8_t cpl);
        void Set(const uint16_t data, std::function<struct _GdtGate*(uint16_t)> GetGdtGate, std::function<struct _GdtGate*(uint16_t)> GetLdtGate, bool protected_mode);
        void On32bitMode();
};