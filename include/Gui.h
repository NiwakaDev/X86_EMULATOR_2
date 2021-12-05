#pragma once
#include "common.h"

class Vga;
class Kbc;
class Mouse;

class Gui:public Object{
    public:
        Gui(Vga* vga, Kbc* kbc, Mouse* mouse);
        ~Gui();
        void Display();
        void SetFont(unsigned char ascii_code, int row, int col);
        void SetFont(unsigned char ascii_code);
        void Update();
        unsigned char font_ascii[128][16];   
    private:    
        int mouse_x, mouse_y;
        Vga* vga = NULL;
        Kbc* kbc = NULL;
        Mouse* mouse = NULL;
        Pixel* vram_mem = NULL;
        void InitFontAscii();
        int screen_width;
        int screen_height;
        SDL_Window* window = NULL;
        SDL_Surface* screenSurface = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Texture *texture   = NULL;
        SDL_DisplayMode display_mode;
        Pixel* image = NULL;
        void HandleKeyDown(SDL_Event *e);
        void HandleKeyUp(SDL_Event *e);
        uint8_t SdlScancode2KeyCode(SDL_Event *e);
        void HandleMouseMotion(SDL_Event *e);
        void HandleMouseButton(SDL_Event *e);
        void HideCursor();
        void ShowCursor();
        int  GetModState();//左ctrl、左altの状態を得る。
        void Resize();
        bool grab;
};