#pragma once
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "Object.h"

struct _Pixel {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} __attribute__((__packed__));

typedef struct _Pixel Pixel;

#define SECTOR_SIZE 512
#define BYTE 8
#define WORD 16
#define DWORD 32