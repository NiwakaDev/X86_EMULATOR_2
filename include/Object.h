#pragma once
#include <stdarg.h>
#include<iostream>
#include<vector>
#include<string>
#include <string.h>

//TODO : クラス名をエラー関連にする
class Object{
    public:
        void Error(const char* message, ...) const;
};