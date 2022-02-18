#pragma once
enum SEGMENT_REGISTER {ES, CS, SS, DS, FS, GS, SEGMENT_REGISTER_COUNT};
enum GENERAL_PURPOSE_REGISTER32 {EAX , ECX, EDX, EBX, ESP, EBP, ESI, EDI, GENERAL_PURPOSE_REGISTER32_COUNT};
enum CONTROL_REGISTER {CR0, CR1, CR2, CR3, CR4, CONTROL_REGISTERS_COUNT};
enum PREFIX_FLG_KIND{
    FLG_3E=0x3E, 
    FLG_67=0x67, 
    FLG_66=0x66, 
    FLG_F0=0xF0, 
    FLG_F2=0xF2, 
    FLG_F3=0xF3, 
    FLG_2E=0x2E, 
    FLG_36=0x36, 
    FLG_26=0x26, 
    FLG_64=0x64, 
    FLG_65=0x65, 
    PREFIX_FLG_KIND_COUNT=10,
};
enum EFLAGS_KIND{
    PF = 1<<2,
    ZF = 1<<6, 
    CF = 1<<0, 
    SF = 1<<7, 
    OF = 1<<11, 
    IF = 1<<9, 
    DF = 1<<10, 
    AC = 1<<18, 
    TF = 1<<8, 
    AF = 1<<4,
    EFLAGS_KIND_CNT=9,
};

namespace CpuEnum{
    enum VECTOR_NUMBER{
        UD = 6,
        NP = 11,
        SS_VECTOR = 12,
        GP = 13,
        VECTOR_NUMBER_CNT=2,
    };
    enum GATE_DESC_TYPE{
        CALL_GATE = 0x0C,
        GATE_DESC_TYPE_CNT=1,
    };
    enum ACCESS_RIGHT{
        P = 1<<7,
        S = 1<<4,
        ACESS_RIGHT = 1,
    };
    enum TYPE_FIELD{
        READ_ONLY       = 0x00,
        READ_AND_ACCESS = 0x01,
    };
};

