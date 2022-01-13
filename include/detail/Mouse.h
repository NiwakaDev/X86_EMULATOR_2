#include "../Mouse.h"

inline void Mouse::SetEnable(){
    this->enable_flg = true;
}

inline void Mouse::Push(uint8_t data){
    if(this->enable_flg)this->fifo->Push(data);
}