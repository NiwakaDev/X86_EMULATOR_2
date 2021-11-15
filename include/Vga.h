#pragma once
#include "common.h"
#define VRAM_START_ADDR 0x000a0000
class Memory;

class Vga:public Object{
    public:
        void Execute();
        void GetVramMode();
        Pixel* GetPixel(int x, int y);
        Vga(Memory *mem);
        uint8_t now_palette_number;
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
    private:
        uint8_t palette[256][4];
        Memory* mem;
        int height;
        int width;
        int vram_start_addr = VRAM_START_ADDR;
        void InitPalette();
        void SetColor(int idx, uint32_t color);
};