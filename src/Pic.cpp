#include "Pic.h"
#include "Kbc.h"
#include "Timer.h"
#include "Fifo.h"
using namespace std;

Pic::Pic(){
    for(int i=0; i<16; i++){
        this->irq_list[i] = false;
    }
    this->fifo = new Fifo<uint8_t>();
}

void Pic::Out8(uint16_t addr, uint8_t data){
    switch (addr){
        case PIC1_ICW1:
        case PIC0_ICW1:
            break;
        case PIC0_IMR:
            for(int i=0; i<8; i++){
                this->irq_list[i] = !((data & (1<<i))==(1<<i));
            }
            break;
        case PIC1_IMR:
            for(int i=8; i<16; i++){
                this->irq_list[i] = !(((((unsigned int)data)<<8) & (1<<i))==(1<<i));
            }
            break;
        default:
            this->Error("Not implemented: io_addr=%04X\n", addr);
    }
}

uint8_t Pic::In8(uint16_t addr){
    switch (addr){
        default:
            this->Error("Not implemented addr=0x%04X at Pic::In8", addr);
    }
}

void Pic::SetTimer(){
    if(!this->irq_list[0]){
        return;
    }
    this->fifo->Push(0);
    return;
}


int Pic::HasIrq(Kbc* kbc, Timer* timer){

    /***
    for(int i=0; i<16; i++){
        if((!this->irq_list[i])){
            continue;
        }
        if(this->device[i]->IsEmpty()){
            continue;
        }
        return this->irq_num[i];
    }
    ***/
   
    if(this->irq_list[0]){
        if(!this->fifo->IsEmpty()){
            this->fifo->Pop();
            return 0;
        }
    }
    if(!this->irq_list[1]){
        return -1;
    }
    return kbc->IsEmpty();
}