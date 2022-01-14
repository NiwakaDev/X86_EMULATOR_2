#pragma once
#include <queue>
#include <mutex>

template<typename type>class Fifo{
    public:
        bool IsEmpty();
        type Pop();
        void Push(const type data);
        type Front();//先頭の値を読み込むだけ。Popしない。
    private:
        std::queue<type> q;
        std::mutex fifo_mtx;
};

#include "detail/Fifo.h"