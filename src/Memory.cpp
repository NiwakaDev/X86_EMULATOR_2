#include "Memory.h"
using namespace std;

Memory::Memory(){
    //this->buff = (uint8_t*)malloc(MEM_SIZE);
    this->buff = new uint8_t[MEM_SIZE];
}

Memory::~Memory(){
    delete[] this->buff;
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

void Memory::MemCpy(uint32_t to_addr, uint32_t from_addr, uint32_t size){
    memcpy(this->buff+to_addr, this->buff+from_addr, size);
}