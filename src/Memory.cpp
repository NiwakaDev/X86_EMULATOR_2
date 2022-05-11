#include "Memory.h"
using namespace std;

Memory::Memory(uint32_t mem_size): mem_size_(mem_size){
    this->buff = make_unique<uint8_t[]>(MEM_SIZE);
}

Memory::~Memory(){

}