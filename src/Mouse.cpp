#include "Pic.h"
#include "Mouse.h"

Mouse::Mouse(Pic* pic){
    this->pic = pic;
    assert(this->pic!=NULL);
    this->enable_flg = false;
    this->fifo = new Fifo<uint8_t>();
}

void Mouse::Out8(uint16_t addr, uint8_t data){
    this->Error("Not implmented: Mouse::Out8");
}

uint8_t Mouse::In8(uint16_t addr){
    switch(addr){
        case 0x60:
            return this->fifo->Pop();
        default:
            this->Error("Not implemented: addr=0x%02X at Mouse::In8", addr);
    }
}

bool Mouse::IsEnable(){
    return this->enable_flg;
}

void Mouse::SetEnable(){
    this->enable_flg = true;
}

void Mouse::Send(uint8_t data){
    this->fifo->Push(data);
}

int Mouse::IsEmpty(){
    return this->fifo->IsEmpty()? -1:0x0C;
}