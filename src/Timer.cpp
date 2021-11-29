#include "Timer.h"
#include "Pic.h"
using namespace std;

Timer::Timer(Pic* pic){
    this->enable = false;
    this->pic    = pic;
    this->clock =  119318;
}

void Timer::Out8(uint16_t addr, uint8_t data){
    switch (addr){
        case 0x43:
            this->mode = 0;
            break;
        case 0x40:
            if(this->mode==0){
                this->cycle = 0;
                this->cycle = data;
                this->mode = 1;
                break;
            }else if(this->mode==1){
                this->cycle  = this->cycle | (((uint32_t)data)<<8);  
                this->cycle  = (uint32_t)ceil(((double)this->clock) / ((double)this->cycle));
                this->mode   = 3;
                this->enable = true;
                this->timer_thread =  new thread(&Timer::Run, this);
                break;
            }
        default:
            this->Error("Not implemented: io_addr(0x%02X) at Timer::Out8");
            break;
    }
}

uint8_t Timer::In8(uint16_t addr){
    this->Error("Not implemented: Timer::In8");
}

void Timer::Run(){
    while(this->enable){
        this_thread::sleep_for(std::chrono::milliseconds(this->cycle));
        this->pic->SetTimer();
    }
}