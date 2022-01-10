#include "../Memory.h"

inline template<typename type>void Memory::Write(const uint32_t addr, const type data){
    if((MEM_SIZE-sizeof(data)+1)<=addr){
        return;
    }
    *(type*)(this->buff+addr) = data;
}

inline uint8_t Memory::Read8(const uint32_t addr) const{
    if((MEM_SIZE)<=addr){
        return 0;
    }
    return this->buff[addr];
}

inline uint16_t Memory::Read16(const uint32_t addr) const{
    if((MEM_SIZE-1)<=addr){
        return 0;
    }
    uint16_t* data = (uint16_t*)(this->buff+addr);
    return *data;
}

inline uint32_t Memory::Read32(const uint32_t addr) const{
    if((MEM_SIZE-3)<=addr){
        return 0;
    }
    uint32_t* data = (uint32_t*)(this->buff+addr);
    return *data;
}

inline uint8_t* Memory::GetPointer(const uint32_t addr) const{
    return this->buff+addr;
}