#include "Memory.h"
#include "Vga.h"

#define ALPHA_IDX 0
#define RED_IDX   1
#define GREEN_IDX 2
#define BLUE_IDX  3
#define RGB_OFFSET 1
#define PALETTE_SIZE 16
#define ALPHA_MAX 0xFF

Vga::Vga(Memory* mem){
    Pixel pixel;
    this->mem = mem;
    this->height = HEIGHT;
    this->width  = WIDTH;
    this->InitPalette();
}

void Vga::Out8(uint16_t addr, uint8_t data){
    static int internal_cnt = 0;
    switch(addr){
        case 0x3C8:
            this->now_palette_number = data;
            break;
        case 0x3C9:
            if(internal_cnt==3){
                this->now_palette_number++;
                internal_cnt = 0;
            }
            this->palette[this->now_palette_number][internal_cnt+RGB_OFFSET] = data<<2;
            internal_cnt++;
            break;
        default:
            this->Error("Not implemented: io_port = %04X at Vga::Out8", addr);
    }
}

uint8_t Vga::In8(uint16_t addr){
    switch(addr){
        default:
            this->Error("Not implemented: io_port = %04X at Vga::In8", addr);
    }
}

Pixel* Vga::GetPixel(int x, int y){
    uint32_t addr = this->vram_start_addr+x+y*this->width;
    return (Pixel*)(this->palette+this->mem->Read8(addr));
}

void Vga::SetColor(int idx, uint32_t color){
    this->palette[idx][RED_IDX] = (uint8_t)((color&0x00FF0000) >> 16);
    this->palette[idx][GREEN_IDX] = (uint8_t)((color&0x0000FF00) >> 8);
    this->palette[idx][BLUE_IDX] = (uint8_t)((color&0x000000FF));
}

void Vga::InitPalette(){
    for(int i=0; i<PALETTE_SIZE; i++){
        this->palette[i][ALPHA_IDX] = ALPHA_MAX;
    }
    this->SetColor(0, 0x00000000);
    this->SetColor(1, 0x000000ff);
    this->SetColor(2, 0x0000ff00);
    this->SetColor(3, 0x0000ffff);
    this->SetColor(4, 0x00ff0000);
    this->SetColor(5, 0x00ff00ff);
    this->SetColor(6, 0x00ffff00);
    this->SetColor(7, 0x00ffffff);
    this->SetColor(8, 0xff000000);
    this->SetColor(9, 0xff0000ff);
    this->SetColor(10,0xff00ff00);
    this->SetColor(11, 0xff00ffff);
    this->SetColor(12,0xffff0000);
    this->SetColor(13, 0xffff00ff);
    this->SetColor(14, 0xffffff00);
    this->SetColor(15, 0xffffffff);
}