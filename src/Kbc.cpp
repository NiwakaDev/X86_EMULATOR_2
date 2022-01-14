#include "Pic.h"
#include "Kbc.h"
#include "Mouse.h"
using namespace std;

const uint8_t ACK = 0xFA;

Kbc::Kbc(Mouse& mouse):IoDevice(){
    this->mouse = &mouse;
}

Kbc::~Kbc(){

}

void Kbc::ProcessCommand(uint8_t command){
    switch (command){
        case 0xF4:
            this->mouse->SetEnable();
            this->fifo->Push(ACK);
            break;
        case 0x47:
            break;
        case 0xED:
            break;
        case 0x00:
            break;
        default:
            this->Error("Not implemented: command = 0x%02X at Kbd::ProcessCommand", command);
            break;
    }
}

void Kbc::Out8(const uint16_t addr, const uint8_t data){
    switch (addr){
        case 0x60:
            this->ProcessCommand(data);
            break;
        case 0x64:
            break;
        default:
            this->Error("Not implemented: addr=0x%04X at Kbc::Out8", addr);
    }
}

uint8_t Kbc::In8(const uint16_t addr){
    uint8_t data;
    switch(addr){
        case 0x60://MOUSE割り込み優先にする
            if(this->mouse->IsEmpty()!=-1){
                data = this->mouse->In8(addr);
                break;
            }
            data = this->fifo->Pop();
            break;
        default:
            this->Error("Not implemented: io_addr=0x%04X at Kbd::In8", addr);
    }
    return data;
}


int Kbc::IsEmpty(){
    if(this->mouse->IsEmpty()!=-1){
        return 0x0C;
    }
    if(!this->fifo->IsEmpty()){
        return 0x01;
    }
    return -1;
}