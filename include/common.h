#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <string.h>
#include <map>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cassert>
#include <stdarg.h>
#include <utility>
#include <condition_variable>
#include <cstdint>
#include <queue>
#include <fstream>
#include <memory>
#include <fstream>
#include "Object.h"

struct _Pixel{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
}__attribute__((__packed__));

typedef struct _Pixel Pixel;

#define SECTOR_SIZE 512
#define BYTE 8
#define WORD 16
#define DWORD 32