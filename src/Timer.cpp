#include "Timer.h"
#include "IoSpace.h"
using namespace std;
using namespace chrono;
using namespace this_thread;

Timer::Timer():IoDevice(){
    this->enable = false;
    this->timer_thread = nullptr;
}

Timer::~Timer(){
    if(this->timer_thread!=nullptr){
        this->enable = false;
        this->timer_thread->join();//それまでのタイマースレッドを終了させる。
        delete this->timer_thread;
    }
}

void Timer::Out8(const uint16_t addr, const uint8_t data){
    switch (addr){
        static const uint32_t clock = 119318;
        static uint32_t mode;
        case PIT_MODE_COMMAND_REGISTER:
            mode = 0;
            this->enable = false;
            if(this->timer_thread!=nullptr){
                this->timer_thread->join();//それまでのタイマースレッドを終了させる。
                delete this->timer_thread;
            }
            break;
        case PIT_CHANNEL_0:
            if(mode==0){
                this->cycle = 0;
                this->cycle = data;
                mode = 1;
                break;
            }else if(mode==1){
                this->cycle  = this->cycle | (((uint32_t)data)<<8);  
                this->cycle  = (uint32_t)ceil(((double)clock) / ((double)this->cycle));
                mode   = 3;
                this->enable = true;
                this->timer_thread =  new thread(&Timer::Run, this);
                break;
            }
        default:
            this->Error("Not implemented: io_addr(0x%02X) at Timer::Out8");
            break;
    }
}

uint8_t Timer::In8(const uint16_t addr){
    this->Error("Not implemented: Timer::In8");
    return 0;
}

void Timer::Run(){
    while(this->enable){
        sleep_for(milliseconds(this->cycle));
        this->Push(0);
    }
}

int Timer::IsEmpty(){
    if(!this->fifo->IsEmpty()){
        //入出力デバイスの中で、Timerだけなぜここで消費するかというと、OS側では消費しないから。
        this->Pop();
        return 0x00;
    }
    return -1;
}