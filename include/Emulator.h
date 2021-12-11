#include "common.h"

class Bios;
class Memory;
class Cpu;
class IoPort;
class Vga;
class Gui;
class Pic;
class Kbc;
class Mouse;
class Timer;
class IoDevice;

class Emulator:public Object{
    private:
        Bios* bios      = NULL;
        Memory* mem     = NULL;
        Cpu* cpu        = NULL;
        IoPort* io_port = NULL;
        Gui* gui        = NULL;
        Vga* vga        = NULL;
        Pic* pic        = NULL;
        Kbc* kbc        = NULL;
        Mouse* mouse    = NULL;
        Timer* timer    = NULL;
        IoDevice* io_devices[16];
        std::thread* emu_thread = NULL;
        int ParseArgv(int argc, char* argv[]);
        char* disk_image_name = NULL;
        bool    debug   = false;
        bool head_start = false;
    public:
        Emulator(int argc, char* argv[]);
        void Run();
};