#include "IoDevice.h"
using namespace std;

IoDevice::IoDevice(){
    this->fifo = make_unique<Fifo<uint8_t>>();
    this->obj  = make_unique<Object>();
}

IoDevice::~IoDevice(){

}

void IoDevice::Push(uint8_t data){
    this->fifo->Push(data);
}

uint8_t IoDevice::Pop(){
    return this->fifo->Pop();
}

uint8_t IoDevice::Front(){
    return this->fifo->Front();
}