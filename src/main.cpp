#include "Emulator.h"
#include "common.h"

using namespace std;

int main(int argc, char* argv[]) {
  Emulator emu(argc, argv);
  emu.RunMainLoop();
  emu.RunGuiThread();
  emu.ThreadJoin();
}