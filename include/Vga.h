#pragma once
#include "common.h"

class Memory;

//#define DEFAULT_HEIGHT 200
//#define DEFAULT_WIDTH 320
//#define DEFAULT_VRAM_START_ADDR 0x000a0000

//#define DEFAULT_HEIGHT 480
//#define DEFAULT_WIDTH 640
//#define DEFAULT_VRAM_START_ADDR 0xfd000000

#define DEFAULT_HEIGHT 400
#define DEFAULT_WIDTH 640
#define DEFAULT_VRAM_START_ADDR 0xfd000000

//	0x101 :  640 x  480 x 8bitカラー
//	0x103 :  800 x  600 x 8bitカラー
//	0x105 : 1024 x  768 x 8bitカラー
//	0x107 : 1280 x 1024 x 8bitカラー

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
        int vram_start_addr;
        void InitPalette();
        void SetColor(int idx, uint32_t color);
};