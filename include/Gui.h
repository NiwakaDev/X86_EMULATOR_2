#pragma once
#include "common.h"

class Vga;
class IoDevice;

class Gui{
    public:
        Gui(Vga& vga);
        ~Gui();
        void Display();
        bool IsQuit();
        void Finish();
        enum IO_DEVICE_KIND {KBD, MOUSE, IO_DEVICE_KIND_TOTAL};
        void AddIoDevice(IO_DEVICE_KIND io_device_kind, IoDevice& io_device);
    private:    
        class Pimpl;
        std::unique_ptr<Pimpl> pimpl;
};

#include "detail/Gui.h"