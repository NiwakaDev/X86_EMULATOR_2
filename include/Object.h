#pragma once
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

// TODO : クラス名をエラー関連にする
// TODO : 
class Object {
 public:
  void Error(const char* message, ...) const;
  //TODO : ObjectクラスをFormatクラスに変更
  std::string Format(const char* messages, ...);
};
