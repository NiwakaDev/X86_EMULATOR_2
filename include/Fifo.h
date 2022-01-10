#pragma once
#include "common.h"

template<typename type>class Fifo{
    public:
        inline bool IsEmpty();
        inline type Pop();
        inline void Push(const type data);
        inline type Front();//先頭の値を読み込むだけ。Popしない。
        std::mutex fifo_mtx;
    private:
        std::queue<type> q;
};

#include "detail/Fifo.h"