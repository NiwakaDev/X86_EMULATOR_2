#pragma once
#include "common.h"

class Memory;

const int MODE13_HEIGHT = 200;
const int MODE13_WIDTH  = 320;
const int MODE13_VRAM_START_ADDR = 0x000a0000;

/***
テキストモードのメモ
１フォントのサイズ : 8x8
解像度           : 640x200
VRAM_START_ADDR : 0x000B8000
文字数           : 80x25
***/

const int DEFAULT_HEIGHT = 200;
const int DEFAULT_WIDTH  = 640;
const int DEFAULT_VRAM_START_ADDR = 0x000B8000;

enum VGA_MODE {GRAPHIC_MODE, TEXT_MODE};

class Vga{
    public:
        VGA_MODE vga_mode;
        std::unique_ptr<Pixel[]> image_text_mode;
        Pixel* GetPixel(const int x, const int y);
        Vga(Memory& mem);
        void Out8(const uint16_t addr, const uint8_t data);
        uint8_t In8(const uint16_t addr);
        void SetInfo(const int width, const int height, const int vram_start_add);
        void SetText(const uint8_t ascii_code, const int w, const int h);//テキストモードのための関数
        int GetWidth();
        int GetHeight();
        int GetVramStartAddr();
        void LockVga();
        void UnlockVga();
        VGA_MODE GetMode();
        void SetSnap(uint8_t* const snap, const int w, const int h);
    private:
        std::unique_ptr<Object> obj;
        std::mutex vga_mtx;
        uint8_t palette[256][4];
        Memory* mem;
        int height;
        int width;
        int vram_start_addr;
        void InitPalette();
        void SetColor(const int idx, const uint32_t color);
        Vga(const Vga& other);
        void operator=(const Vga& other);
};

#include "detail/Vga.h"