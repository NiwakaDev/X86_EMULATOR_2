#pragma once
#include "common.h"
#include "Fifo.h"

enum PIC_REGISTERS{
    PIC0_ICW1	= 	0x0020,
    PIC0_OCW2	= 	0x0020,
    PIC0_IMR	=   0x0021,
    PIC0_ICW2	= 	0x0021, 
    PIC0_ICW3	= 	0x0021,
    PIC0_ICW4	= 	0x0021,
    PIC1_ICW1	= 	0x00a0,
    PIC1_OCW2	= 	0x00a0,
    PIC1_IMR	=   0x00a1,
    PIC1_ICW2	= 	0x00a1,
    PIC1_ICW3	= 	0x00a1,
    PIC1_ICW4	= 	0x00a1,
};

class Kbc;
class Timer;
class IoDevice;

class Pic:public Object{
    public:
        void Execute();
        Pic(IoDevice** io_devices);
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int HasIrq(Kbc* kbc, Timer* timer); //TODO : Pic::HasIrqの戻り値をboolに変更
        Fifo<uint8_t>* fifo = NULL;
    private:
        IoDevice**io_devices = NULL;
        bool irq_list[16];//割り込み許可
        int now_irq;
};