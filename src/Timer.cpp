#include "Timer.h"
#include "Pic.h"
#include "IoSpace.h"
using namespace std;
using namespace chrono;
using namespace this_thread;

Timer::Timer(Pic* pic){
    this->enable = false;
    this->pic    = pic;
    this->clock =  119318;
    this->timer_thread = nullptr;
}

void Timer::Out8(uint16_t addr, uint8_t data){
    switch (addr){
        case PIT_MODE_COMMAND_REGISTER:
            this->mode = 0;
            this->enable = false;
            if(this->timer_thread!=nullptr){
                this->timer_thread->join();//それまでのタイマースレッドを終了させる。
                delete this->timer_thread;
            }
            break;
        case PIT_CHANNEL_0:
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
    return 0;
}

void Timer::Run(){
    while(this->enable){
        sleep_for(milliseconds(this->cycle));
        this->pic->SetTimer();
    }
}