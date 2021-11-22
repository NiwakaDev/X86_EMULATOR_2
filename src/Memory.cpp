#include "Memory.h"

Memory::Memory(){
    this->buff = (uint8_t*)malloc(MEM_SIZE);
    assert(this->buff!=NULL);
}

void Memory::Dump(uint32_t start, uint32_t size){
    for(int i=0; i<size; i++){
        if(i%16==0){
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "%02X ", this->Read8(start+i));
    }
    fprintf(stderr, "\n");
}

uint8_t Memory::Read8(uint32_t addr){
    return this->buff[addr];
}

uint16_t Memory::Read16(uint32_t addr){
    uint16_t* data = (uint16_t*)(this->buff+addr);
    return *data;
}

uint32_t Memory::Read32(uint32_t addr){
    uint32_t* data = (uint32_t*)(this->buff+addr);
    return *data;
}

uint8_t* Memory::GetPointer(uint32_t addr){
    return this->buff+addr;
}