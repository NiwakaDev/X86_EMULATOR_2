#include "../Vga.h"

inline int Vga::GetWidth(){
    return this->width;
}

inline int Vga::GetHeight(){
    return this->height;
}

inline int Vga::GetVramStartAddr(){
    return this->vram_start_addr;
}

inline void Vga::LockVga(){
    this->vga_mtx.lock();
}

inline void Vga::UnlockVga(){
    this->vga_mtx.unlock();
}

inline VGA_MODE Vga::GetMode(){
    return this->vga_mode;
}