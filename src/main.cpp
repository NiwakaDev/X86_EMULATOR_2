#include "common.h"
#include "Emulator.h"

using namespace std;

int main(int argc, char* argv[]){
    Emulator emu(argc, argv);
    emu.RunMainLoop();
    emu.ThreadJoin();
}