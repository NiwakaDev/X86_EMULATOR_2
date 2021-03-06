#include "Timer.h"
#include <cmath>
#include "IoSpace.h"
using namespace std;
using namespace chrono;
using namespace this_thread;

Timer::Timer() {
  this->fifo = make_unique<Fifo<uint8_t>>();
  this->obj = make_unique<Object>();
  this->enable = false;
  this->timer_thread = nullptr;
}

Timer::~Timer() {
  if (this->timer_thread.get() == nullptr) {
    return;
  }
  this->enable = false;
  if (this->timer_thread->joinable()) {
    this->timer_thread->join();
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
      if (this->timer_thread.get() == nullptr) {
        return;
      }
      if (this->timer_thread->joinable()) {
        this->timer_thread->join();
      }
      this->timer_thread.reset(nullptr);
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
        this->timer_thread = make_unique<thread>(&Timer::Run, this, cycle);
        return;
      }
    default:
      throw runtime_error(this->obj->Format(
          "Not implemented: io_addr(0x%04X) at Timer::Out8", addr));
      break;
  }
}

uint8_t Timer::In8(const uint16_t addr) {
  throw runtime_error("Not implemented: Timer::In8");
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

void Timer::Push(uint8_t data) { this->fifo->Push(data); }

uint8_t Timer::Pop() { return this->fifo->Pop(); }

uint8_t Timer::Front() { throw runtime_error("Not implemented: Timer::Front"); }