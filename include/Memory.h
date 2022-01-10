#pragma once
#include "common.h"
const int MEM_SIZE = 33554432;//2<<24
class Memory:public Object{
    private:
        uint8_t* buff;
    public:
        Memory();
        ~Memory();
        template<typename type>void Write(const uint32_t addr, const type data);
        uint8_t Read8(const uint32_t addr) const;
        uint16_t Read16(const uint32_t addr) const;
        uint32_t Read32(const uint32_t addr) const;
        uint8_t* GetPointer(const uint32_t addr) const;
};

#include "detail/Memory.h"