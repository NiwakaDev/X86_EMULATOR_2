#pragma once
#include "common.h"
#include "Fifo.h"

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021 
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

class Kbc;
class Timer;

class Pic:public Object{
    public:
        void Execute();
        Pic();
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int HasIrq(Kbc* kbc, Timer* timer);
        Fifo<uint8_t>* fifo = NULL;
        void SetTimer();
    private:
        bool irq_list[16];//割り込み許可
        int now_irq;
};