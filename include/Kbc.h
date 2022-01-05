#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;
class Mouse;

enum KEY_CODE {
    KEY_CODE_0 = 0x0b, 
    KEY_CODE_1 = 0x02,
    KEY_CODE_2 = 0x03, 
    KEY_CODE_3 = 0x04,
    KEY_CODE_4 = 0x05,
    KEY_CODE_5 = 0x06, 
    KEY_CODE_6 = 0x07,
    KEY_CODE_7 = 0x08,
    KEY_CODE_8 = 0x09,
    KEY_CODE_9 = 0x0A,
    KEY_CODE_A = 0x1E,
    KEY_CODE_B = 0x30,
    KEY_CODE_C = 0x2E,
    KEY_CODE_D = 0x20,
    KEY_CODE_E = 0x12,
    KEY_CODE_F = 0x21,
    KEY_CODE_G = 0x22,
    KEY_CODE_H = 0x23,
    KEY_CODE_I = 0x17,
    KEY_CODE_J = 0x24,
    KEY_CODE_K = 0x25,
    KEY_CODE_L = 0x26,
    KEY_CODE_M = 0x32,
    KEY_CODE_N = 0x31,
    KEY_CODE_O = 0x18,
    KEY_CODE_P = 0x19,
    KEY_CODE_Q = 0x10,
    KEY_CODE_R = 0x13,
    KEY_CODE_S = 0x1F,
    KEY_CODE_T = 0x14,
    KEY_CODE_U = 0x16,
    KEY_CODE_V = 0x2F,
    KEY_CODE_W = 0x11,
    KEY_CODE_X = 0x2D,
    KEY_CODE_Y = 0x15,
    KEY_CODE_Z = 0x2C,
    KEY_CODE_SEMICOLON = 0x27,
    KEY_CODE_COLON = 0x28,
    KEY_CODE_CAPSLOCK = 0x3A,
    KEY_CODE_SPACE = 0x39,
    KEY_CODE_ENTER = 0x1C,
    KEY_CODE_PERIOD = 0x34,
    KEY_CODE_COMMA = 0x33,
    KEY_CODE_LEFTBRACKET = 0x1B,
    KEY_CODE_RIGHTBRACKET = 0x2B,
    KEY_CODE_BACKSPACE = 0x0E,
    KEY_CODE_LSHIFT  = 0x2A,
    KEY_CODE_RSHIFT = 0x36,
    KEY_CODE_LALT = 0x38,
    KEY_CODE_TAB = 0x0F,
    KEY_CODE_LCTRL = 0x1D,
    KEY_CODE_MINUS = 0x4A,
    KEY_CODE_F1 = 0x3B,
    KEY_CODE_F2 = 0x3C,
    KEY_CODE_F3 = 0x3D,
    KEY_CODE_F4 = 0x3E,
    KEY_CODE_F5 = 0x3F,
    KEY_CODE_F6 = 0x40,
    KEY_CODE_F7 = 0x41,
    KEY_CODE_F8 = 0x42,
    KEY_CODE_F9 = 0x43,
    KEY_CODE_F10 = 0x44,
    KEY_CODE_F11 = 0x57,
    KEY_CODE_F12 = 0x58,
    KEY_CODE_AT = 0x1A,
    KEY_CODE_SLASH = 0x35,
    KEY_CODE_UNDERSCORE = 0x73,
    KEY_CODE_BACKSLASH = 0x7D,
};

enum {
    KEY_CODE_MAKE  = 0x00,
    KEY_CODE_BREAK = 0x80,
};

//packet format
//https://wiki.osdev.org/Mouse_Input#PS2_Mouse_--_Basic_Operation_.28Microsoft_compliant.29
enum PACEKT_INFO{
    DEFAULT_PACKET_BYTE0 = 0x08,
    X_SIGN_BIT = 0x10,
    Y_SIGN_BIT = 0x20,
    LEFT_BUTTON = 0x01,
};

class Kbc:public IoDevice{
    private:
        Mouse* mouse = NULL;
        void ProcessCommand(uint8_t command);
    public:
        Kbc(Mouse* mouse);
        void Out8(const uint16_t addr, const uint8_t data);
        uint8_t In8(const uint16_t addr);
        int IsEmpty();
};