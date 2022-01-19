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
class Fdc;
class IoDevice;

class Emulator:public Object{
    public:
        Memory* mem     = NULL;
        Cpu* cpu        = NULL;
        IoPort* io_port = NULL;
        Emulator(int argc, char* argv[]);
        ~Emulator();
        void ThreadJoin();
        void RunMainLoop();
    private:
        Bios* bios      = NULL;
        Gui* gui        = NULL;
        Vga* vga        = NULL;
        Pic* pic        = NULL;
        Kbc* kbc        = NULL;
        Mouse* mouse    = NULL;
        Timer* timer    = NULL;
        Fdc* fdc        = NULL;
        IoDevice* io_devices[16];
        std::thread* emu_thread = NULL;
        int ParseArgv(int argc, char* argv[]);
        const char* disk_image_name = NULL;
        const char* bios_name       = NULL;//いずれBIOSクラスを外す予定だが、当分は共存させる。
        bool head_start = false;
        void MainLoop();
        Emulator(const Emulator& other);
        void operator=(const Emulator& other);
};