#include "../Fifo.h"

inline template <typename type>
bool Fifo<type>::IsEmpty() {
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  return this->q.empty();
}

inline template <typename type>
type Fifo<type>::Pop() {
  type element;
  if (this->IsEmpty()) {
    return element;
  }
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  element = this->q.front();
  this->q.pop();
  return element;
}

inline template <typename type>
void Fifo<type>::Push(const type data) {
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  if (this->q.size() == 16) {
    return;
  }
  this->q.push(data);
  return;
}

inline template <typename type>
type Fifo<type>::Front() {  //読み込むだけ
  type element;
  if (this->IsEmpty()) {
    return element;
  }
  std::lock_guard<std::mutex> lock(this->fifo_mtx);
  element = this->q.front();
  return element;
}