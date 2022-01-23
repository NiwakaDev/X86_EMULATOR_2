#pragma once
#include <stdarg.h>
#include<iostream>
#include<vector>
#include<string>
#include <string.h>

class Object{
    public:
        void Error(const char* message, ...) const;
        ~Object(){}//ポリモーフィズムとして利用しないので、仮想デストラクタにしない。
};