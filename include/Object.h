#pragma once
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

// TODO : クラス名をエラー関連にする
class Object {
 public:
  void Error(const char* message, ...) const;
};