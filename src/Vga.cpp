#include "Memory.h"
#include "Vga.h"

extern uint8_t font8x8_basic[128][8];
using namespace std;

#define ALPHA_IDX 0
#define RED_IDX   1
#define GREEN_IDX 2
#define BLUE_IDX  3
#define RGB_OFFSET 1
#define PALETTE_SIZE 16
#define ALPHA_MAX 0xFF


Vga::Vga(Memory* mem){
    this->mem = mem;
    this->height = DEFAULT_HEIGHT;
    this->width  = DEFAULT_WIDTH;
    this->vram_start_addr = DEFAULT_VRAM_START_ADDR;
    this->InitPalette();
    this->vga_mode = TEXT_MODE;
    this->image_text_mode = (Pixel*)malloc(this->height*this->width*sizeof(Pixel));//最大領域の場合のサイズで確保しておく。
}

//この関数はVgaクラスのvga_mutexをロックします。
void Vga::SetInfo(int width, int height, int vram_start_addr){
    this->vga_mtx.lock();
    this->width = width;
    this->height = height;
    this->vram_start_addr = vram_start_addr;
    this->vga_mode = GRAPHIC_MODE;
    this->vga_mtx.unlock();
}

void Vga::Out8(uint16_t addr, uint8_t data){
    static int internal_cnt = 0;
    static uint8_t now_palette_idx;
    switch(addr){
        case 0x3C8:
            now_palette_idx = data;
            internal_cnt    = 0;
            break;
        case 0x3C9:
            if(internal_cnt==3){
                now_palette_idx++;
                internal_cnt = 0;
            }
            this->palette[now_palette_idx][internal_cnt+RGB_OFFSET] = data<<2;
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
    return 0;
}

void Vga::SetText(uint8_t ascii_code, int w, int h){
    //テキストモード用変数領域の始まり  後でメンバ化予定
    int row = 0;
    int col = 0;
    int x, y;
    x = w*8;
    y = h*8;
    char data;
    uint8_t *font = font8x8_basic[ascii_code];
    Pixel pixel;
    pixel.r = 0xFF;
    pixel.g = 0xFF;
    pixel.b = 0xFF;

    Pixel black_pixel;
    black_pixel.r = 0x00;
    black_pixel.g = 0x00;
    black_pixel.b = 0x00;
    //テキストモード用変数領域の終わり

    for(int i=0; i < 8; i++){
        data = font[i];
        if((data & 0x80) != 0){
            this->image_text_mode[x+7+(y+i)*this->width] = pixel; 
        }else{
            this->image_text_mode[x+7+(y+i)*this->width] = black_pixel; 
        }
        if((data & 0x40) != 0){
            this->image_text_mode[x+6+(y+i)*this->width] = pixel;    
        }else{
            this->image_text_mode[x+6+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x20) != 0){
            this->image_text_mode[x+5+(y+i)*this->width] = pixel;      
        }else{
            this->image_text_mode[x+5+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x10) != 0){
            this->image_text_mode[x+4+(y+i)*this->width] = pixel;     
        }else{
            this->image_text_mode[x+4+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x08) != 0){
            this->image_text_mode[x+3+(y+i)*this->width] = pixel;     
        }else{
            this->image_text_mode[x+3+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x04) != 0){
            this->image_text_mode[x+2+(y+i)*this->width] = pixel;    
        }else{
            this->image_text_mode[x+2+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x02) != 0){
            this->image_text_mode[x+1+(y+i)*this->width] = pixel; 
        }else{
            this->image_text_mode[x+1+(y+i)*this->width] = black_pixel; 
        }

        if((data & 0x01) != 0){
            this->image_text_mode[x+0+(y+i)*this->width] = pixel;  
        }else{
            this->image_text_mode[x+0+(y+i)*this->width] = black_pixel; 
        }
    }
}

Pixel* Vga::GetPixel(int x, int y){
    if(this->vga_mode==TEXT_MODE){
        return &this->image_text_mode[x+y*this->width];
    }
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

void Vga::SetSnap(uint8_t* snap, int w, int h){
    memcpy(snap, this->mem->GetPointer(this->vram_start_addr), this->width*this->height);
}