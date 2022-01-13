#pragma once
#include "common.h"
#include <SDL2/SDL_mixer.h>

class Vga;
class IoDevice;

class Gui:public Object{
    public:
        Gui(Vga& vga);
        ~Gui();
        void Display();
        bool IsQuit();
        void Finish();
        void SoundFdc();
        enum IO_DEVICE_KIND {KBD, MOUSE, IO_DEVICE_KIND_TOTAL};
        void AddIoDevice(IO_DEVICE_KIND io_device_kind, IoDevice& io_device);
    private:    
        class Impl;
        Impl* impl;
};

#include "detail/Gui.h"