#pragma once
#include "common.h"

template<typename type>class Fifo{
    private:
        std::queue<type> q;
    public:
        bool IsEmpty(){
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            return this->q.empty();
        }
        type Pop(){
            type element;
            if(this->IsEmpty()){
                return element;
            }
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            element = this->q.front();
            this->q.pop();
            return element;
        }
        void Push(type data){
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            this->q.push(data);
            return;
        }
        std::mutex fifo_mtx;
};