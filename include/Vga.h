#pragma once
#include "common.h"

class Memory;

#define DEFAULT_HEIGHT 200
#define DEFAULT_WIDTH 320
#define DEFAULT_VRAM_START_ADDR 0x000a0000

/***
テキストモードのメモ
１フォントのサイズ : 8x8
解像度           : 640x200
VRAM_START_ADDR : 0x000B8000
文字数           : 80x25
***/

class Vga:public Object{
    public:
        void Execute();
        void GetVramMode();
        Pixel* GetPixel(int x, int y);
        Vga(Memory *mem);
        uint8_t now_palette_number;
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        mutex vga_mtx;
        void SetInfo(int width, int height, int vram_start_add);
        int GetWidth();
        int GetHeight();
    private:
        uint8_t palette[256][4];
        Memory* mem;
        int height;
        int width;
        int vram_start_addr;
        void InitPalette();
        void SetColor(int idx, uint32_t color);
};