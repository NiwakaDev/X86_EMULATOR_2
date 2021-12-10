#pragma once
#include "common.h"
#include "Fifo.h"

class Pic;
class Mouse;

#define KEY_CODE_0 0x0b
#define KEY_CODE_1 0x02
#define KEY_CODE_2 0x03
#define KEY_CODE_3 0x04
#define KEY_CODE_4 0x05
#define KEY_CODE_5 0x06
#define KEY_CODE_6 0x07
#define KEY_CODE_7 0x08
#define KEY_CODE_8 0x09
#define KEY_CODE_9 0x0A
#define KEY_CODE_A 0x1E
#define KEY_CODE_B 0x30
#define KEY_CODE_C 0x2E
#define KEY_CODE_D 0x20
#define KEY_CODE_E 0x12
#define KEY_CODE_F 0x21
#define KEY_CODE_G 0x22
#define KEY_CODE_H 0x23
#define KEY_CODE_I 0x17
#define KEY_CODE_J 0x24
#define KEY_CODE_K 0x25
#define KEY_CODE_L 0x26
#define KEY_CODE_M 0x32
#define KEY_CODE_N 0x31
#define KEY_CODE_O 0x18
#define KEY_CODE_P 0x19
#define KEY_CODE_Q 0x10
#define KEY_CODE_R 0x13
#define KEY_CODE_S 0x1F
#define KEY_CODE_T 0x14
#define KEY_CODE_U 0x16
#define KEY_CODE_V 0x2F
#define KEY_CODE_W 0x11
#define KEY_CODE_X 0x2D
#define KEY_CODE_Y 0x15
#define KEY_CODE_Z 0x2C
#define KEY_CODE_SEMICOLON 0x27
#define KEY_CODE_COLON 0x28
#define KEY_CODE_CAPSLOCK 0x3A
#define KEY_CODE_SPACE 0x39
#define KEY_CODE_ENTER 0x1C
#define KEY_CODE_PERIOD 0x34
#define KEY_CODE_COMMA 0x33
#define KEY_CODE_LEFTBRACKET 0x1B
#define KEY_CODE_RIGHTBRACKET 0x2B
#define KEY_CODE_BACKSPACE 0x0E
#define KEY_CODE_LSHIFT 0x2A
#define KEY_CODE_RSHIFT 0x36
#define KEY_CODE_LALT 0x38
#define KEY_CODE_TAB 0x0F
#define KEY_CODE_LCTRL 0x1D
#define KEY_CODE_MINUS 0x4A
#define KEY_CODE_F1 0x3B
#define KEY_CODE_F2 0x3C
#define KEY_CODE_F3 0x3D
#define KEY_CODE_F4 0x3E
#define KEY_CODE_F5 0x3F 
#define KEY_CODE_F6 0x40
#define KEY_CODE_F7 0x41
#define KEY_CODE_F8 0x42
#define KEY_CODE_F9 0x43
#define KEY_CODE_F10 0x44
#define KEY_CODE_F11 0x57
#define KEY_CODE_F12 0x58

#define KEY_CODE_MAKE 0x00
#define KEY_CODE_BREAK 0x80

//packet format
//https://wiki.osdev.org/Mouse_Input#PS2_Mouse_--_Basic_Operation_.28Microsoft_compliant.29
#define DEFAULT_PACKET_BYTE0 0x08
#define X_SIGN_BIT 0x10
#define Y_SIGN_BIT 0x20
#define LEFT_BUTTON 0x01

class Kbc:public Object{
    private:
        Pic* pic = NULL;
        Mouse* mouse = NULL;
        Fifo<uint8_t>* fifo = NULL;
        void ProcessCommand(uint8_t command);
    public:
        Kbc(Pic* pic, Mouse* mouse);
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int IsEmpty();
        void Push(uint8_t data);
        uint8_t Pop();//リアルモードで使用する
        uint8_t Front();//読み込むだけ。リアルモードで使用する
};