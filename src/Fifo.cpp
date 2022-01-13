#include "Fifo.h"

using namespace std;

template<typename type> bool Fifo<type>::IsEmpty(){
    lock_guard<mutex> lock(this->fifo_mtx);
    return this->q.empty();
}

template<typename type> type Fifo<type>::Pop(){
    type element;
    if(this->IsEmpty()){
        return element;
    }
    lock_guard<mutex> lock(this->fifo_mtx);
    element = this->q.front();
    this->q.pop();
    return element;
}

template<typename type> void Fifo<type>::Push(const type data){
    lock_guard<mutex> lock(this->fifo_mtx);
    if(this->q.size()==16){
        return;
    }
    this->q.push(data);
    return;
}

template<typename type> type Fifo<type>::Front(){//読み込むだけ
    type element;
    if(this->IsEmpty()){
        return element;
    }
    lock_guard<mutex> lock(this->fifo_mtx);
    element = this->q.front();
    return element;
}

//明示的インスタンス化
template class Fifo<uint8_t>;