#pragma once
#include "common.h"
const int MEM_SIZE = 33554432;//2<<24
class Memory:public Object{
    private:
        uint8_t* buff;
    public:
        Memory();
        ~Memory();
        template<typename type>void Write(const uint32_t addr, const type data){
            if((MEM_SIZE-sizeof(data)+1)<=addr){
                return;
            }
            *(type*)(this->buff+addr) = data;
        }
        void MemCpy(uint32_t to_addr, uint32_t from_addr, uint32_t size);
        uint8_t Read8(const uint32_t addr) const{
            if((MEM_SIZE)<=addr){
                return 0;
            }
            return this->buff[addr];
        }
        uint16_t Read16(const uint32_t addr) const{
            if((MEM_SIZE-1)<=addr){
                return 0;
            }
            uint16_t* data = (uint16_t*)(this->buff+addr);
            return *data;
        }
        uint32_t Read32(const uint32_t addr) const{
            if((MEM_SIZE-3)<=addr){
                return 0;
            }
            uint32_t* data = (uint32_t*)(this->buff+addr);
            return *data;
        }
        uint8_t* GetPointer(const uint32_t addr) const{
            return this->buff+addr;
        }
        void Dump(uint32_t start, uint32_t size);
};  