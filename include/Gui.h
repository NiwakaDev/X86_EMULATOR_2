#pragma once
#include "common.h"

class Vga;

class Gui{
    public:
        Gui();
        ~Gui();
        void Display(Vga& vga);
        bool IsQuit();
        void Finish();
        enum IO_DEVICE_KIND {KBD, MOUSE, IO_DEVICE_KIND_TOTAL};
        void AddIoDevice(IO_DEVICE_KIND io_device_kind, std::function<void(uint8_t data)> push);
    private:    
        class Pimpl;
        std::unique_ptr<Pimpl> pimpl;
};

#include "detail/Gui.h"