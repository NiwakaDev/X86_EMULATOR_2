#include "common.h"
#include "Emulator.h"

using namespace std;

int main(int argc, char* argv[]){
    Emulator* emu = new Emulator(argc, argv);
    emu->Start();
    emu->Join();
}