#include "../Fifo.h"

using namespace std;

inline template<typename type> bool Fifo<type>::IsEmpty(){
    lock_guard<mutex> lock(this->fifo_mtx);
    return this->q.empty();
}

inline template<typename type> type Fifo<type>::Pop(){
    type element;
    if(this->IsEmpty()){
        return element;
    }
    lock_guard<mutex> lock(this->fifo_mtx);
    element = this->q.front();
    this->q.pop();
    return element;
}

inline template<typename type> void Fifo<type>::Push(const type data){
    lock_guard<mutex> lock(this->fifo_mtx);
    if(this->q.size()==16){
        return;
    }
    this->q.push(data);
    return;
}

inline template<typename type> type Fifo<type>::Front(){//読み込むだけ
    type element;
    if(this->IsEmpty()){
        return element;
    }
    lock_guard<mutex> lock(this->fifo_mtx);
    element = this->q.front();
    return element;
}