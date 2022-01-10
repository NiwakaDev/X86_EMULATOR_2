#include "../Mouse.h"

inline bool Mouse::IsEnable(){
    return this->enable_flg;
}

inline void Mouse::SetEnable(){
    this->enable_flg = true;
}