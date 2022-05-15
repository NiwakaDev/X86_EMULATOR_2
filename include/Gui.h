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
        //SoundFdc関数は外す予定。とりあえず、コメントアウトとして残しておく。
        //void SoundFdc();
        enum IO_DEVICE_KIND {KBD, MOUSE, IO_DEVICE_KIND_TOTAL};
        void AddIoDevice(IO_DEVICE_KIND io_device_kind, IoDevice& io_device);
    private:    
        class Pimpl;
        std::unique_ptr<Pimpl> pimpl;
};

#include "detail/Gui.h"