#pragma once
#include "common.h"

template<typename type>class Fifo{
    public:
        bool IsEmpty();
        type Pop();
        void Push(const type data);
        type Front();//先頭の値を読み込むだけ。Popしない。
        std::mutex fifo_mtx;
    private:
        std::queue<type> q;
};

//uint8_tを実引数に取るFifoクラステンプレートしか、インスタンス化できません。
typedef Fifo<uint8_t> Uint8Fifo;