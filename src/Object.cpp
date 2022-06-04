#include "Object.h"
#include <memory>
using namespace std;

//TODO: これは廃止にし、ErrorをFormatに置き換える。
void Object::Error(const char* message, ...) const {
  char* message_with_ln = (char*)malloc(strlen(message) + 2);
  strcpy(message_with_ln, message);
  message_with_ln[strlen(message) + 1] = '\0';
  message_with_ln[strlen(message)] = '\n';
  va_list ap;
  va_start(ap, message_with_ln);
  vfprintf(stderr, message_with_ln, ap);
  va_end(ap);
  free(message_with_ln);
  throw " ";  // TODO このメッセージは無意味なので、変更予定
}

string Object::Format(const char* message, ...){
  //2048よりも大きいメッセージは使わないはず。
  const int buff_size = 2048;
  auto buff = make_unique<char[]>(buff_size);
  va_list ap;
  va_start(ap, message);
  vsprintf(buff.get(), message, ap);
  va_end(ap);
  return string(buff.get());
}

