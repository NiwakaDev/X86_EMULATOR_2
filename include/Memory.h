#pragma once
#include "common.h"
#define MEM_SIZE 4294967296

class Memory:public Object{
    private:
        uint8_t* buff;
    public:
        Memory();
        template<typename type>void Write(uint32_t addr, type data){
            uint8_t* p = (uint8_t*)&data;
            for(int i=0; i<sizeof(data); i++){
                this->buff[addr+i] = p[i];
            }
        }
        uint8_t  Read8(uint32_t addr);
        uint16_t Read16(uint32_t addr);
        uint32_t Read32(uint32_t addr);
        uint8_t* GetPointer(uint32_t addr);
        void Dump(uint32_t start, uint32_t size);
};  