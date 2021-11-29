#include "Sib.h"
#include "Cpu.h"
using namespace std;

void Sib::ParseSib(uint8_t sib_byte, uint8_t mod){
    assert(this!=NULL);
    this->scale = ((sib_byte & 0xc0)>>6);
    this->index = ((sib_byte & 0x38)>>3);
    this->base  = sib_byte & 0x07;
    this->mod   = mod;
}

uint32_t Sib::GetAddress(Cpu* cpu){
    assert(this!=NULL);
    uint32_t addr = 0;
    if(this->base==4){
        cpu->SetDataSelector(SS);
    }
    if(this->base==5&&(this->mod==1 || this->mod==2)){
        addr = cpu->GetR32(EBP);
        cpu->SetDataSelector(SS);

    }
    else if(this->base==5 && this->mod==0x00){
        addr = this->disp32;
    }

    if(this->scale==0){
        if(this->base!=5){
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
        }
        if(this->index!=4){
            addr = addr + cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->index);
        }
    }
    if(this->scale==1){
        if(this->base!=5){
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
        }
        if(this->index!=4){
            addr = addr + cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->index)*2;
        }
    }
    if(this->scale==2){
        if(this->base!=5){
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
        }
        if(this->index!=4){
            addr = addr + cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->index)*4;
        }
    }
    if(this->scale==3){
        if(this->base!=5){
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
        }
        if(this->index!=4){
            addr = addr + cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->index)*8;
        }
    }
    return addr;
}

uint32_t Sib::GetBase(){
    return this->base;
}

uint32_t Sib::GetScale(){
    return this->scale;
}

uint32_t Sib::GetIndex(){
    return this->index;
}

void Sib::SetDisp32(uint32_t disp32){
    this->disp32 = disp32;
}