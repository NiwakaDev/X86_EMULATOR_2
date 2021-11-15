#pragma once
#include <stdarg.h>
#include<iostream>
#include<vector>
#include<string>
#include <string.h>
using namespace std;
class Object{
    public:
        void Error(const char* message, ...);
};