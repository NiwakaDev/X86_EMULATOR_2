#include "../Fifo.h"

template <typename type>
inline bool Fifo<type>::IsEmpty() {
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  return this->q.empty();
}

template <typename type>
inline type Fifo<type>::Pop() {
  type element;
  if (this->IsEmpty()) {
    return element;
  }
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  element = this->q.front();
  this->q.pop();
  return element;
}

template <typename type>
inline void Fifo<type>::Push(const type data) {
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  if (this->q.size() == 16) {
    return;
  }
  this->q.push(data);
  return;
}

template <typename type>
inline type Fifo<type>::Front() {  //読み込むだけ
  type element;
  if (this->IsEmpty()) {
    return element;
  }
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  element = this->q.front();
  return element;
}