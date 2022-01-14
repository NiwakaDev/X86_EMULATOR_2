#include "Pic.h"
#include "Kbc.h"
#include "Timer.h"
#include "Fifo.h"
using namespace std;

Pic::Pic(IoDevice** io_devices){
    for(int i=0; i<16; i++){
        this->irq_list[i] = false;
    }
    this->io_devices = io_devices;
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

//TODO : Pic::HasIrqの戻り値をboolに変更
int Pic::HasIrq(){
    int irq_num;
    for(int i=0; i<16; i++){
        if((!this->irq_list[i])||(this->io_devices[i]==NULL)){
            continue;
        }
        if((irq_num=this->io_devices[i]->IsEmpty())==-1){
            continue;
        }
        return irq_num;
    }
    return -1;
}