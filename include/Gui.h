#pragma once
#include "common.h"

class Gui{
    public:
        Gui(int default_height, int default_width);
        ~Gui();
        void Display(std::function<void(Pixel* image, int* display_width, int* display_height, std::function<void()> resize_callback)> set_image_callback);
        bool IsQuit();
        void Finish();
        enum IO_DEVICE_KIND {KBD, MOUSE, IO_DEVICE_KIND_TOTAL};
        void AddIoDevice(IO_DEVICE_KIND io_device_kind, std::function<void(uint8_t data)> push);
    private:    
        class Pimpl;
        std::unique_ptr<Pimpl> pimpl;
};

#include "detail/Gui.h"