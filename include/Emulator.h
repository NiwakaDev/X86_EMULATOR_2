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

class Emulator{
    public:
        std::unique_ptr<Memory> mem;
        std::unique_ptr<Cpu> cpu;
        std::unique_ptr<IoPort> io_port;
        Emulator(int argc, char* argv[]);
        ~Emulator();
        void ThreadJoin();
        void RunMainLoop();
        void RunGuiThread();
        #ifdef DEBUG
            int steps=-1;
        #endif
    private:
        std::unique_ptr<Object> obj;
        std::unique_ptr<Bios> bios;
        std::unique_ptr<Gui> gui;
        std::unique_ptr<Vga> vga; 
        std::unique_ptr<Pic> pic;
        std::unique_ptr<Kbc> kbc;
        std::unique_ptr<Mouse> mouse;
        std::unique_ptr<Timer> timer;
        std::unique_ptr<Fdc> fdc;
        IoDevice* io_devices[16];
        std::unique_ptr<std::thread> emu_thread;
        int ParseArgv(int argc, char* argv[]);
        const char* disk_image_name = NULL;
        const char* bios_name       = NULL;//いずれBIOSクラスを外す予定だが、当分は共存させる。
        bool head_start = false;
        void MainLoop();
        Emulator(const Emulator& other);
        void operator=(const Emulator& other);
};