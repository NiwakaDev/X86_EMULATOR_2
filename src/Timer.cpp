#include "Timer.h"
#include <cmath>
#include "IoSpace.h"
using namespace std;
using namespace chrono;
using namespace this_thread;

Timer::Timer() : IoDevice() {
  this->enable = false;
  this->timer_thread = nullptr;
}

Timer::~Timer() {
  if (this->timer_thread != nullptr) {
    this->enable = false;
    this->timer_thread->join();
    delete this->timer_thread;
  }
}

void Timer::Out8(const uint16_t addr, const uint8_t data) {
  switch (addr) {
    static const uint32_t clock = 119318;
    static uint32_t cycle;
    static uint32_t mode;
    case PIT_MODE_COMMAND_REGISTER:
      mode = 0;
      this->enable = false;
      if (this->timer_thread != nullptr) {
        this->timer_thread->join();
        delete this->timer_thread;
      }
      return;
    case PIT_CHANNEL_0:
      if (mode == 0) {
        cycle = 0;
        cycle = data;
        mode = 1;
        return;
      } else if (mode == 1) {
        cycle = cycle | (((uint32_t)data) << 8);
        cycle = (uint32_t)ceil(((double)clock) / ((double)cycle));
        mode = 3;
        this->enable = true;
        this->timer_thread = new thread(&Timer::Run, this, cycle);
        return;
      }
    default:
      this->obj->Error("Not implemented: io_addr(0x%02X) at Timer::Out8");
      break;
  }
}

uint8_t Timer::In8(const uint16_t addr) {
  this->obj->Error("Not implemented: Timer::In8");
  return 0;
}

void Timer::Run(uint32_t cycle) {
  while (this->enable) {
#ifdef DEBUG
    //デバッグ状態では、レジスタ状態を再現性のあるものにしたい。
    //だから、何もしない。
#else
    sleep_for(milliseconds(cycle));
    this->Push(0);
#endif
  }
}

// TODO : Timer::IsEmptyの戻り値をboolに変更
int Timer::IsEmpty() {
  if (this->fifo->IsEmpty()) {
    return -1;
  }
  //入出力デバイスの中で、Timerだけなぜここで消費するかというと、OS側では消費しないから。
  this->Pop();
  return 0x00;
}