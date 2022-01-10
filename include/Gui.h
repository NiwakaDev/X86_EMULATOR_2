#pragma once
#include "common.h"
#include <SDL2/SDL_mixer.h>

class Vga;
class Kbc;
class Mouse;

class Gui:public Object{
    public:
        Gui(Vga& vga, Kbc& kbc, Mouse& mouse);
        ~Gui();
        void Display();
        bool IsQuit();
        void Finish();
        void SoundFdc();
    private:    
        class Impl;
        Impl* impl;
};

#include "detail/Gui.h"