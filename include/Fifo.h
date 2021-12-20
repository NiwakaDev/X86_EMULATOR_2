#pragma once
#include "common.h"

template<typename type>class Fifo{
    private:
        std::queue<type> q;
    public:
        inline bool IsEmpty(){
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            return this->q.empty();
        }
        inline type Pop(){
            type element;
            if(this->IsEmpty()){
                return element;
            }
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            element = this->q.front();
            this->q.pop();
            return element;
        }
        inline void Push(type data){
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            if(this->q.size()==16){
                return;
            }
            this->q.push(data);
            return;
        }
        inline type Front(){//読み込むだけ
            type element;
            if(this->IsEmpty()){
                return element;
            }
            std::lock_guard<std::mutex> lock(this->fifo_mtx);
            element = this->q.front();
            return element;
        }
        std::mutex fifo_mtx;
};