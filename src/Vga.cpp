#include "Vga.h"
#include "Font.h"

extern uint8_t font8x8_basic[128][8];
using namespace std;

static const int ALPHA_IDX = 0;
static const int RED_IDX = 1;
static const int GREEN_IDX = 2;
static const int BLUE_IDX = 3;
static const int RGB_OFFSET = 1;
static const int PALETTE_SIZE = 16;
static const int ALPHA_MAX = 0xFF;

Vga::Vga(std::function<uint8_t(const uint32_t addr)> mem_read8) {
  this->mem_read8 = mem_read8;
  this->height = DEFAULT_HEIGHT;
  this->width = DEFAULT_WIDTH;
  this->vram_start_addr = DEFAULT_VRAM_START_ADDR;
  this->InitPalette();
  this->vga_mode = TEXT_MODE;
  this->image_text_mode =
      make_unique<Pixel[]>(this->height * this->width * sizeof(Pixel));
  this->obj = make_unique<Object>();
}

//この関数はVgaクラスのvga_mutexをロックします。
void Vga::SetInfo(const int width, const int height,
                  const int vram_start_addr) {
  lock_guard<mutex> lock(this->vga_mtx);
  this->width = width;
  this->height = height;
  this->vram_start_addr = vram_start_addr;
  this->vga_mode = GRAPHIC_MODE;
}

void Vga::Out8(const uint16_t addr, const uint8_t data) {
  switch (addr) {
    static int internal_cnt = 0;
    static uint8_t now_palette_idx;
    case 0x3C8:
      now_palette_idx = data;
      internal_cnt = 0;
      break;
    case 0x3C9:
      if (internal_cnt == 3) {
        now_palette_idx++;
        internal_cnt = 0;
      }
      this->palette[now_palette_idx][internal_cnt + RGB_OFFSET] = data << 2;
      internal_cnt++;
      break;
    default:
      this->obj->Error("Not implemented: io_port = %04X at Vga::Out8", addr);
  }
}

uint8_t Vga::In8(const uint16_t addr) {
  switch (addr) {
    default:
      this->obj->Error("Not implemented: io_port = %04X at Vga::In8", addr);
  }
  return 0;
}

void Vga::SetText(const uint8_t ascii_code, const int w, const int h) {
  int x = w * 8;
  int y = h * 8;

  uint8_t* font = font8x8_basic[ascii_code];
  Pixel pixel = {
    .a = 0x00, 
    .r = 0xFF, 
    .g = 0xFF, 
    .b = 0xFF, 
  };
  Pixel black_pixel = {
    .a = 0x00, 
    .r = 0x00,
    .g = 0x00, 
    .b = 0x00,  
  };
  for (int i = 0; i < 8; i++) {
    uint8_t data = font[i];
    if ((data & 0x80) != 0) {
      this->image_text_mode[x + 7 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 7 + (y + i) * this->width] = black_pixel;
    }
    if ((data & 0x40) != 0) {
      this->image_text_mode[x + 6 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 6 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x20) != 0) {
      this->image_text_mode[x + 5 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 5 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x10) != 0) {
      this->image_text_mode[x + 4 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 4 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x08) != 0) {
      this->image_text_mode[x + 3 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 3 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x04) != 0) {
      this->image_text_mode[x + 2 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 2 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x02) != 0) {
      this->image_text_mode[x + 1 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 1 + (y + i) * this->width] = black_pixel;
    }

    if ((data & 0x01) != 0) {
      this->image_text_mode[x + 0 + (y + i) * this->width] = pixel;
    } else {
      this->image_text_mode[x + 0 + (y + i) * this->width] = black_pixel;
    }
  }
}

Pixel* Vga::GetPixel(const int x, const int y) {
  if (this->vga_mode == TEXT_MODE) {
    return &this->image_text_mode[x + y * this->width];
  }
  uint32_t addr = this->vram_start_addr + x + y * this->width;
  return (Pixel*)(this->palette + this->mem_read8(addr));
}

inline void Vga::SetColor(const int idx, const uint32_t color) {
  this->palette[idx][RED_IDX] = (uint8_t)((color & 0x00FF0000) >> 16);
  this->palette[idx][GREEN_IDX] = (uint8_t)((color & 0x0000FF00) >> 8);
  this->palette[idx][BLUE_IDX] = (uint8_t)((color & 0x000000FF));
}

void Vga::InitPalette() {
  for (int i = 0; i < PALETTE_SIZE; i++) {
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
  this->SetColor(10, 0xff00ff00);
  this->SetColor(11, 0xff00ffff);
  this->SetColor(12, 0xffff0000);
  this->SetColor(13, 0xffff00ff);
  this->SetColor(14, 0xffffff00);
  this->SetColor(15, 0xffffffff);
}

void Vga::SetImage(Pixel* image, int* display_width, int* display_height,
                   std::function<void()> resize_callback) {
  lock_guard<mutex> lock(this->vga_mtx);
  if ((this->GetHeight() != (*display_height)) ||
      (this->GetWidth() != (*display_width))) {
    *display_height = this->GetHeight();
    *display_width = this->GetWidth();
    resize_callback();
  }
  for (int y = 0; y < this->height; y++) {
    for (int x = 0; x < this->width; x++) {
      image[x + y * this->width] = *(this->GetPixel(x, y));
    }
  }
}