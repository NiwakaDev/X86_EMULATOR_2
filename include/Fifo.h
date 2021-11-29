#pragma once
#include "common.h"
using namespace std;

template<typename type>class Fifo{
    private:
        queue<type> q;
    public:
        bool IsEmpty(){
            lock_guard<mutex> lock(this->fifo_mtx);
            return this->q.empty();
        }
        type Pop(){
            type element;
            if(this->IsEmpty()){
                return element;
            }
            lock_guard<mutex> lock(this->fifo_mtx);
            element = this->q.front();
            this->q.pop();
            return element;
        }
        void Push(type data){
            lock_guard<mutex> lock(this->fifo_mtx);
            this->q.push(data);
            return;
        }
        mutex fifo_mtx;
};