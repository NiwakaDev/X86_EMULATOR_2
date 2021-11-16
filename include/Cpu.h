#pragma once
#include "common.h"
#include "Instruction.h"

class SegmentRegister;
class Memory;
class Instruction;
class Bios;
class IoPort;
class Gdtr;
class Idtr;
class Ldtr;
class TaskRegister;

enum SEGMENT_REGISTER {ES, CS, SS, DS, FS, GS, SEGMENT_REGISTER_COUNT};
enum GENERAL_PURPOSE_REGISTER32 {EAX , ECX, EDX, EBX, ESP, EBP, ESI, EDI, GENERAL_PURPOSE_REGISTER32_COUNT};
//enum PREFIX_FLG_KIND {FLG_67, FLG_66, FLG_F0, FLG_F2, FLG_F3, FLG_2E, FLG_36, FLG_26, FLG_64, FLG_65, PREFIX_FLG_KIND_COUNT};
enum PREFIX_FLG_KIND {FLG_67=0x67, FLG_66=0x66, FLG_F0=0xF0, FLG_F2=0xF2, FLG_F3=0xF3, FLG_2E=0x2E, FLG_36=0x36, FLG_26=0x26, FLG_64=0x64, FLG_65=0x65, PREFIX_FLG_KIND_COUNT=10};
enum EFLAGS_KIND {ZF, CF, SF, OF, IF, DF, EFLAGS_KIND_CNT};
enum CONTROL_REGISTER {CR0, CR1, CR2, CR3, CR4, CONTROL_REGISTERS_COUNT};

#define REAL_MODE 0     //REAL_MODEの挙動：「IA-32 インテル® アーキテクチャソフトウェア・デベロッパーズ・マニュアル下巻：システム・プログラミング・ガイド」16章
#define PROTECT_MODE 1
#define IPL_START_ADDR 0x7c00

#define SIGN_FLG1 0x00000080
#define SIGN_FLG2 0x00008000
#define SIGN_FLG4 0x80000000
#define TSS_TYPE 0x09
#define LDT_TYPE 0x0082
#define SEGMENT_DESC_TYPE_FLG 1<<4

#define DPL 0x60 //access_rightのDPLの該当部分
#define GET_DPL(data) (data&DPL)>>5
#define GET_RPL(data) (data&0x03)

typedef struct _GdtGate{
    uint16_t limit_low, base_low;
    uint8_t base_mid, access_right;
    uint8_t limit_high, base_high;
}__attribute__((__packed__));

typedef struct _IdtGate{
    uint16_t offset_low, selector;
    uint8_t dw_cnt, access_right;
    uint16_t offset_high;
}__attribute__((__packed__));  

typedef struct _Tss{
	uint32_t backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	uint32_t es, cs, ss, ds, fs, gs;
	uint32_t ldtr, iomap;
}__attribute__((__packed__));

typedef _GdtGate GdtGate;
typedef _IdtGate IdtGate;
typedef _Tss Tss;

#define DB 0x40


class Cpu:public Object{
    private:
        Bios* bios = NULL;
        Gdtr* gdtr = NULL;
        Idtr* idtr = NULL;
        Ldtr* ldtr = NULL;
        Memory* mem = NULL;
        Instruction* instructions[INSTRUCTION_SIZE];
        SEGMENT_REGISTER default_code_selector;
        SEGMENT_REGISTER default_data_selector;
        SEGMENT_REGISTER default_stack_selector;
        map<uint8_t, bool> prefix_table;//1byteがprefixかどうかを示すテーブル
        map<uint8_t, bool> prefix_flgs;//現在実行中の機械語命令で使用しているプレフィックスを管理
        //bool prefix_flgs[PREFIX_FLG_KIND_COUNT];
        //制御レジスタ : 「IA-32 インテル® アーキテクチャソフトウェア・デベロッパーズ・マニュアル下巻：システム・プログラミング・ガイド」2.5節を参照
        union{
            uint32_t raw;
            struct{
                unsigned PE:1;
                unsigned MP:1;
                unsigned EM:1;
                unsigned TS:1;
                unsigned ET:1;
                unsigned NE:1;
                unsigned reserve1:10;
                unsigned WP:1;
                unsigned reserve2:1;
                unsigned AM:1;
                unsigned reserve3:10;
                unsigned NW:1;
                unsigned CD:1;
                unsigned PG:1;
            }flgs;
        }cr0;
        uint32_t eip = IPL_START_ADDR;
        uint32_t gprs[GENERAL_PURPOSE_REGISTER32_COUNT];
        SegmentRegister* segment_registers[SEGMENT_REGISTER_COUNT];
        TaskRegister* task_register;
        union{
            uint32_t raw;
            struct{
                unsigned CF : 1;
                unsigned RESERVED0 : 1;
                unsigned PF : 1;
                unsigned RESERVED1 : 1;
                unsigned AF : 1;
                unsigned RESERVED2 : 1;
                unsigned ZF : 1;
                unsigned SF : 1;
                unsigned TF : 1;
                unsigned IF : 1;
                unsigned DF : 1;
                unsigned OF : 1;
                unsigned IOPL : 2;
                unsigned NT : 1;
                unsigned RESERVED3 : 1;
                unsigned RF : 1;
                unsigned VM : 1;
                unsigned AC : 1;
                unsigned VIF : 1;
                unsigned VIP : 1;
                unsigned ID : 1;
            }flgs;
        }eflags;
        void InitSelector();
        void ResetPrefixFlg();
        void CheckPrefixCode(Memory* mem);
        void Push32(uint32_t data);
    public:
        Cpu(Bios* bios, Memory* mem);
        void Run(IoPort* io_port);
        void AddEip(uint32_t data);
        void AddIp(uint16_t data);
        void SetEip(uint32_t addr);
        uint32_t GetLinearAddrForCodeAccess();
        uint32_t GetLinearAddrForDataAccess(uint32_t offset);
        uint32_t GetLinearStackAddr();
        bool IsProtectedMode();
        bool Is32bitsMode();
        bool IsPrefixAddrSize();
        bool IsPrefixOpSize();
        bool IsPrefixRep();
        bool IsPrefixRepnz();
        void SetR8L(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data);
        void SetR8H(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data);
        void SetR8(uint32_t register_type, uint8_t data);
        void SetR16(SEGMENT_REGISTER register_type, uint16_t data);
        void SetR16(GENERAL_PURPOSE_REGISTER32 register_type, uint16_t data);
        void SetR32(GENERAL_PURPOSE_REGISTER32 register_type, uint32_t data);
        void SetGdtr(uint16_t limit, uint32_t base);
        void SetIdtr(uint16_t limit, uint32_t base);
        void SetCr(CONTROL_REGISTER control_register_type, uint32_t data);
        void SetTr(uint16_t selector);
        void SetDataSelector(SEGMENT_REGISTER register_type);
        void SetEflgs(uint32_t eflgs);
        uint8_t  GetR8(uint32_t register_type);
        uint8_t  GetR8L(GENERAL_PURPOSE_REGISTER32 register_type);
        uint8_t  GetR8H(GENERAL_PURPOSE_REGISTER32 register_type);
        uint16_t GetR16(GENERAL_PURPOSE_REGISTER32 register_type);
        uint16_t GetR16(SEGMENT_REGISTER register_type);
        uint32_t GetR32(GENERAL_PURPOSE_REGISTER32 register_type);
        uint32_t GetEflgs();
        uint32_t GetEip();
        uint16_t GetIp();
        uint32_t GetCr(CONTROL_REGISTER control_register_type);
        uint16_t GetLdtr();
        uint32_t GetGdtBaseAddr();
        uint32_t GetIdtBase();
        GdtGate* GetGdtGate(uint16_t selector);
        GdtGate* GetLdtGate(uint16_t selector);
        IdtGate* GetIdtGate(uint16_t selector);
        Tss*     GetCurrentTss();
        uint32_t GetBaseAddr(SEGMENT_REGISTER register_type);
        void UpdateEflagsForSub(uint64_t result, uint32_t d1, uint32_t d2);
        void UpdateEflagsForSub8(uint32_t result, uint8_t d1, uint8_t d2);
        void UpdateEflagsForSub16(uint32_t result, uint16_t d1, uint16_t d2);
        void UpdateEflagsForInc(uint32_t result, uint32_t d1, uint32_t d2);
        template<typename type>void UpdateEflagsForDec(type result, type d1, type d2){
            this->UpdateZF(result);
            this->UpdateSF(result);
            this->UpdatePF(result);
            switch(sizeof(result)){
                case 1:
                    this->UpdateOF_Sub8(result, d1, d2);
                    break;
                case 4:
                    this->UpdateOF_Sub(result, d1, d2);
                    break;
                default:
                    this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateElfagsForDec", sizeof(result));
            }
        }
        template<typename type1, typename type2>void UpdateEflagsForAdd(type1 result, type2 d1, type2 d2){
            this->UpdateZF(result);
            this->UpdateSF(result);
            this->UpdatePF(result);
            this->UpdateCF(result);
            switch(sizeof(d1)){
                case 1:
                    this->UpdateOF_Add((uint8_t)result, (uint8_t)d1, (uint8_t)d2);
                    break;
                case 2:
                    this->UpdateOF_Add((uint16_t)result, (uint16_t)d1, (uint16_t)d2);
                    break;
                case 4:
                    this->UpdateOF_Add((uint32_t)result, (uint32_t)d1, (uint32_t)d2);
                    break;
                default:
                    this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF", sizeof(result));
            }
        }
        template<typename type>void UpdateEflagsForShr(type result){
            this->UpdateZF((uint32_t)result);
            this->UpdateSF(result);
            this->UpdatePF((uint32_t)result);
        }
        template<typename type>void UpdateOF_Add(type result, type d1, type d2){
            switch(sizeof(result)){
                case 1:
                    this->eflags.flgs.OF = ((d1&SIGN_FLG1)==(d2&SIGN_FLG1)) && ((result&SIGN_FLG1)!=(d1&SIGN_FLG1));
                    break;
                case 2:
                    this->eflags.flgs.OF = ((d1&SIGN_FLG2)==(d2&SIGN_FLG2)) && ((result&SIGN_FLG2)!=(d1&SIGN_FLG2));
                    break;
                case 4:
                    this->eflags.flgs.OF = ((d1&SIGN_FLG4)==(d2&SIGN_FLG4)) && ((result&SIGN_FLG4)!=(d1&SIGN_FLG4));
                    break;
                default:
                    this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF", sizeof(result));
            }
        }
        void UpdateOF_Sub(uint32_t result, uint32_t d1, uint32_t d2);
        void UpdateOF_Sub16(uint16_t result, uint16_t d1, uint16_t d2);
        void UpdateOF_Sub8(uint8_t result, uint8_t d1, uint8_t d2);
        void UpdateZF(uint32_t result);
        void UpdateCF(uint64_t result);
        void UpdatePF(uint32_t result);
        template<typename type> void UpdateSF(type data){
            switch(sizeof(data)){
                case 1:
                    this->eflags.flgs.SF = ((data&SIGN_FLG1)==SIGN_FLG1)? 1:0;
                    break;
                case 2:
                    this->eflags.flgs.SF = ((data&SIGN_FLG2)==SIGN_FLG2)? 1:0;
                    break;
                case 4:
                case 8:
                    this->eflags.flgs.SF = ((data&SIGN_FLG4)==SIGN_FLG4)? 1:0;
                    break;
                default:
                    this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF", sizeof(data));
            }
        }
        bool IsFlag(EFLAGS_KIND eflags_kind);
        void SetFlag(EFLAGS_KIND eflags_kind);
        void ClearFlag(EFLAGS_KIND eflags_kind);
        template<typename type>void UpdateEflagsForAnd(type data){
            this->ClearFlag(CF);
            this->ClearFlag(OF);
            this->UpdateSF(data);
            this->UpdateZF(data);
            this->UpdatePF(data);
        }
        void CallFunctionOnRealMode(Memory* mem, uint8_t selector);
        void HandleInterrupt(int irq_num);
        void SaveTask(uint16_t selector);
        void SwitchTask();
        void ShowSegmentRegisters();
        uint8_t GetCpl();
        bool CheckPrivilegeLevel();
        void SetRpl(SEGMENT_REGISTER register_type, uint8_t rpl);
        void SetCpl(SEGMENT_REGISTER register_type, uint8_t cpl);
};