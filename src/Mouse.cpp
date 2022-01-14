#include "Pic.h"
#include "Mouse.h"
using namespace std;

Mouse::Mouse():IoDevice(){
    this->enable_flg = false;
}

Mouse::~Mouse(){
    
}

void Mouse::Out8(const uint16_t addr, const uint8_t data){
    this->Error("Not implmented: Mouse::Out8");
}

uint8_t Mouse::In8(const uint16_t addr){
    switch(addr){
        case 0x60:
            return this->fifo->Pop();
        default:
            this->Error("Not implemented: addr=0x%02X at Mouse::In8", addr);
    }
}

int Mouse::IsEmpty(){
    return this->fifo->IsEmpty()? -1:0x0C;
}