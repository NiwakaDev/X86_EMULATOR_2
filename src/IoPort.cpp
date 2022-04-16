#pragma once
#include "IoPort.h"
#include "Vga.h"
#include "Pic.h"
#include "Kbc.h"
#include "Timer.h"
#include "Fdc.h"
#include "IoSpace.h"
using namespace std;

//io_map : http://oswiki.osask.jp/?%28AT%29iomap

IoPort::IoPort(Vga& vga, Pic& pic, Kbc& kbc, Timer& timer, Fdc& fdc){
    this->vga   = &vga;
    this->pic   = &pic;
    this->kbc   = &kbc;
    this->timer = &timer;
    this->fdc   = &fdc;
}

//TODO : マジックナンバーを定数化
void IoPort::Out8(const uint16_t addr, const uint8_t data){
    switch(addr){
        case FDC_CONFIGURATION_CONTROL_REGISTER:
        case FDC_DATA_FIFO:
        case FDC_DIGITAL_OUTPUT_REGISTER:
            this->fdc->Out8(addr, data);
            break;
        case 0x61:
            break;
        case PIT_CHANNEL_2:
            break;
        case PIT_CHANNEL_0:
        case PIT_MODE_COMMAND_REGISTER:
            this->timer->Out8(addr, data);
            break;
        case 0x60:
        case 0x64:
            this->kbc->Out8(addr, data);
            break;
        case Pic::PIC1_ICW1:
        case Pic::PIC0_IMR:
        case Pic::PIC1_IMR:
        case Pic::PIC0_ICW1:
            this->pic->Out8(addr, data);
            break;
        case 0x3C8:
        case 0x3C9:
            this->vga->Out8(addr, data);
            break;
        case POST_IO_PORT:
            fprintf(stderr, "POST = 0x%02X\n", data);
            break;
        default:
            //this->Error("Not implemented: io_port = 0x%04X at IoPort::Out8", addr);
            fprintf(stderr, "Not implemented: io_port = 0x%04X ", addr);
            throw "at IoPort::Out8";
    }
}

uint8_t IoPort::In8(const uint16_t addr){
    switch(addr){
        case FDC_DATA_FIFO:
        case FDC_MAIN_STATUS_REGISTER:
            return this->fdc->In8(addr);
        case Pic::PIC1_IMR:
        case Pic::PIC0_IMR:
            return 0;//TODO : いずれ実装予定
        case 0x61:
            return 0;
        case 0x60:
            return this->kbc->In8(addr);
        case 0x064:
            return 0;
        default:
            this->Error("Not implemented: io_port = %04X at IoPort::In8", addr);
    }
}