#include "Fdc.h"
using namespace std;


Fdc::Fdc():IoDevice(){

}

void Fdc::ProcessCommand(uint8_t command){
    switch (command){
        default:
            this->Error("Not implemented: command = 0x%02X at Fdc::ProcessCommand", command);
            break;
    }
}

void Fdc::Out8(uint16_t addr, uint8_t data){
    switch (addr){
        default:
            this->Error("Not implemented: addr=0x%04X at Fdc::Out8", addr);
    }
}

uint8_t Fdc::In8(uint16_t addr){
    uint8_t data;
    switch(addr){
        default:
            this->Error("Not implemented: io_addr=0x%04X at Fdc::In8", addr);
    }
    return data;
}


int Fdc::IsEmpty(){
    if(!this->fifo->IsEmpty()){
        return 0x06;
    }
    return -1;
}