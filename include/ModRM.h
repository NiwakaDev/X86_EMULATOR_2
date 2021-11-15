#pragma once
#include "common.h"
typedef struct _ModRM{
    uint8_t mod;
    union{
        uint8_t op_code;
        uint8_t reg_index;
    };
    uint8_t rm;
    uint8_t sib;
    union{
        int8_t disp8;
        uint16_t disp16;
        uint32_t disp32;
    };
}ModRM;