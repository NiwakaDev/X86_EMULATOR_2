#pragma once
#include "common.h"

class Memory;

/***
#define DEFAULT_HEIGHT 200
#define DEFAULT_WIDTH 320
#define DEFAULT_VRAM_START_ADDR 0x000a0000
***/

/***
テキストモードのメモ
１フォントのサイズ : 8x8
解像度           : 640x200
VRAM_START_ADDR : 0x000B8000
文字数           : 80x25
***/

#define DEFAULT_HEIGHT 200
#define DEFAULT_WIDTH 320
#define DEFAULT_VRAM_START_ADDR 0x000B8000

enum VGA_MODE {GRAPHIC_MODE, TEXT_MODE};

class Vga:public Object{
    public:
        VGA_MODE vga_mode;
        Pixel* image_text_mode=NULL;
        void Execute();
        Pixel* GetPixel(int x, int y);
        Vga(Memory *mem);
        uint8_t now_palette_idx;
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        void SetInfo(int width, int height, int vram_start_add);
        int GetWidth();
        int GetHeight();
        void LockVga();
        void UnlockVga();
        void SetText(uint8_t ascii_code, int w, int h);//テキストモードのための関数
        VGA_MODE GetMode();
    private:
        std::mutex vga_mtx;
        uint8_t palette[256][4];
        Memory* mem;
        int height;
        int width;
        int vram_start_addr;
        void InitPalette();
        void SetColor(int idx, uint32_t color);
};