#include "Memory.h"
using namespace std;

Memory::Memory(){
    this->buff = make_unique<uint8_t[]>(MEM_SIZE);
}

Memory::~Memory(){

}