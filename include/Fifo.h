#pragma once
#include "common.h"
using namespace std;

template<typename TYPE>class Fifo{
    private:
        queue<TYPE> q;
    public:
        bool IsEmpty(){
            lock_guard<std::mutex> lock(this->fifo_mtx);
            return this->q.empty();
        }
        TYPE Pop(){
            TYPE element;
            if(this->IsEmpty()){
                return element;
            }
            lock_guard<mutex> lock(this->fifo_mtx);
            element = this->q.front();
            this->q.pop();
            return element;
        }
        void Push(TYPE data){
            lock_guard<mutex> lock(this->fifo_mtx);
            this->q.push(data);
            return;
        }
        mutex fifo_mtx;
};