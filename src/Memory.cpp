#include "Memory.h"
using namespace std;

Memory::Memory(){
    //this->buff = (uint8_t*)malloc(MEM_SIZE);
    this->buff = new uint8_t[MEM_SIZE];
}

Memory::~Memory(){
    delete[] this->buff;
}