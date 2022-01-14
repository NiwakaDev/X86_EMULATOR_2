#include "Memory.h"
using namespace std;

Memory::Memory(){
    this->buff = new uint8_t[MEM_SIZE];
}

Memory::~Memory(){
    delete[] this->buff;
}