#include "Cpu.h"
#include "SegmentRegister.h"
#include "Memory.h"
#include "Bios.h"
#include "Gdtr.h"
#include "Idtr.h"
#include "Ldtr.h"
#include "TaskRegister.h"
#include "Instruction.h"
#include "InstructionHelper.h"

using namespace std;

const int  EFLAGS_INIT_VALUE = 0x00000002;
const int  EIP_INIT_VALUE    = 0x0000FFF0;
const int  CR0_INIT_VALUE    = 0x60000010;
const int  CR2_INIT_VALUE    = 0x00000000;
const int  CS_INIT_VALUE     = 0x0000F000;

Cpu::Cpu(Bios& bios, Memory& mem){
    this->bios = &bios;
    this->mem  = &mem;
    this->eflags.raw = EFLAGS_INIT_VALUE;
    //this->eip        = IPL_START_ADDR;
    this->eip = EIP_INIT_VALUE;
    #ifdef DEBUG
        this->eflags.raw = 0x0000F002;//8086runの値を参考にした。8086runと比較するために同じにしているだけ。いずれ仕様通りの初期値にする予定
    #else
        this->eflags.raw = EFLAGS_INIT_VALUE;
    #endif
    this->cr0.raw = CR0_INIT_VALUE;
    for(int i=0; i<SEGMENT_REGISTER_COUNT; i++){
        if(i==CS){
            this->segment_registers[CS] = make_unique<SegmentRegister>(CS_INIT_VALUE);
            continue;
        }
        this->segment_registers[i] = make_unique<SegmentRegister>(0x0000);
    }
    this->task_register = make_unique<TaskRegister>(0x00);
    this->ldtr = make_unique<Ldtr>(0);
    this->gdtr = make_unique<Gdtr>("Gdtr", 0, 0);
    this->idtr = make_unique<Idtr>("Idtr", 0, 0);

    this->registers.eax  =0x00000000;
    this->registers.ebx  =0x00000000;
    this->registers.ecx  =0x00000000;
    this->registers.edx  =0x00000000;
    this->registers.esp  =0x00000000;
    this->registers.ebp  =0x00000000;
    this->registers.esi  =0x00000000;
    this->registers.edi  =0x00000000;

    this->gprs[EAX] = &(this->registers.eax);
    this->gprs[EBX] = &(this->registers.ebx);
    this->gprs[ECX] = &(this->registers.ecx);
    this->gprs[EDX] = &(this->registers.edx);
    this->gprs[ESP] = &(this->registers.esp);
    this->gprs[EBP] = &(this->registers.ebp);
    this->gprs[ESI] = &(this->registers.esi);
    this->gprs[EDI] = &(this->registers.edi);



    //TODO : prefixは全てInstructionクラスにする。
    //その機械語にはprefixが適用されるかどうかのフラグ
    //FLG_F3は命令化した。
    //FLG_F2は命令化した。
    this->prefix_flgs[FLG_67] = false;
    this->prefix_flgs[FLG_66] = false;
    this->prefix_flgs[FLG_F0] = false;
    this->prefix_flgs[FLG_2E] = false;
    this->prefix_flgs[FLG_36] = false;
    this->prefix_flgs[FLG_26] = false;
    this->prefix_flgs[FLG_64] = false;
    this->prefix_flgs[FLG_65] = false;

    //1バイトがprefixかどうかを管理するテーブル
    for(int i=0; i<256; i++){
        this->prefix_table[i] = false;
    }
    this->prefix_table[0xF0] = true;
    this->prefix_table[0x26] = true;
    this->prefix_table[0x2E] = true;
    this->prefix_table[0x36] = true;
    this->prefix_table[0x3E] = true;
    this->prefix_table[0x64] = true;
    this->prefix_table[0x65] = true;
    this->prefix_table[0x66] = true;
    this->prefix_table[0x67] = true;

    for(int i=0; i<InstructionHelper::INSTRUCTION_SIZE; i++){
        InstructionHelper::InstructionFactory instruction_factory;
        this->instructions[i] = instruction_factory.CreateInstruction(i);
    }

    /***
    this->bios = &bios;
    this->mem  = &mem;
    this->eflags.raw = EFLAGS_INIT_VALUE;
    this->eip        = IPL_START_ADDR;
    #ifdef DEBUG
        this->eflags.raw = 0x0000F002;//8086runの値を参考にした。8086runと比較するために同じにしているだけ。いずれ仕様通りの初期値にする予定
    #else
        this->eflags.raw = EFLAGS_INIT_VALUE;
    #endif
    this->cr0.raw = CR0_INIT_VALUE;
    for(int i=0; i<SEGMENT_REGISTER_COUNT; i++){
        this->segment_registers[i] = make_unique<SegmentRegister>(0x0000);
    }
    this->task_register = make_unique<TaskRegister>(0x00);
    this->ldtr = make_unique<Ldtr>(0);
    this->gdtr = make_unique<Gdtr>("Gdtr", 0, 0);
    this->idtr = make_unique<Idtr>("Idtr", 0, 0);

    this->registers.eax  =0x00000000;
    this->registers.ebx  =0x00000000;
    this->registers.ecx  =0x00000000;
    this->registers.edx  =0x00000000;
    this->registers.esp  =0x00000000;
    this->registers.ebp  =0x00000000;
    this->registers.esi  =0x00000000;
    this->registers.edi  =0x00000000;

    this->gprs[EAX] = &(this->registers.eax);
    this->gprs[EBX] = &(this->registers.ebx);
    this->gprs[ECX] = &(this->registers.ecx);
    this->gprs[EDX] = &(this->registers.edx);
    this->gprs[ESP] = &(this->registers.esp);
    this->gprs[EBP] = &(this->registers.ebp);
    this->gprs[ESI] = &(this->registers.esi);
    this->gprs[EDI] = &(this->registers.edi);



    //TODO : prefixは全てInstructionクラスにする。
    //その機械語にはprefixが適用されるかどうかのフラグ
    //FLG_F3は命令化した。
    //FLG_F2は命令化した。
    this->prefix_flgs[FLG_67] = false;
    this->prefix_flgs[FLG_66] = false;
    this->prefix_flgs[FLG_F0] = false;
    this->prefix_flgs[FLG_2E] = false;
    this->prefix_flgs[FLG_36] = false;
    this->prefix_flgs[FLG_26] = false;
    this->prefix_flgs[FLG_64] = false;
    this->prefix_flgs[FLG_65] = false;

    //1バイトがprefixかどうかを管理するテーブル
    for(int i=0; i<256; i++){
        this->prefix_table[i] = false;
    }
    this->prefix_table[0xF0] = true;
    this->prefix_table[0x26] = true;
    this->prefix_table[0x2E] = true;
    this->prefix_table[0x36] = true;
    this->prefix_table[0x3E] = true;
    this->prefix_table[0x64] = true;
    this->prefix_table[0x65] = true;
    this->prefix_table[0x66] = true;
    this->prefix_table[0x67] = true;

    for(int i=0; i<InstructionHelper::INSTRUCTION_SIZE; i++){
        InstructionHelper::InstructionFactory instruction_factory;
        this->instructions[i] = instruction_factory.CreateInstruction(i);
    }
    ***/
   this->is_exception_ = false;
}

Cpu::~Cpu(){

}


union{
    uint32_t raw;
    struct{
        unsigned P   : 1;
        unsigned RW  : 1;
        unsigned US  : 1;
        unsigned PWT : 1;
        unsigned PCD : 1;
        unsigned A   : 1;
        unsigned reserve2 : 1;
        unsigned PS  : 1;
        unsigned reserve1 :4;
        unsigned base_addr:20;
    }flgs;
}PDE;

union{
    uint32_t raw;
    struct{
        unsigned P    : 1;
        unsigned RW   : 1;
        unsigned US   : 1;
        unsigned PWT  : 1;
        unsigned PCD  : 1;
        unsigned A    : 1;
        unsigned D    : 1;
        unsigned PAT  : 1;
        unsigned G    : 3;
        unsigned base_addr:20;
    }flgs;
}PTE;

uint32_t Cpu::GetPhysicalAddr(uint32_t linear_addr){
    //TODO : ページングに対応させる。
    if(this->cr0.flgs.PG){
        //TODO : リニアアドレスを共用体にする。
        uint32_t dir_idx;
        uint32_t table_idx;
        uint32_t offset;
        dir_idx   = (linear_addr&0xffc00000)>>22;
        table_idx = (linear_addr&0x003FF000)>>12;
        offset    = (linear_addr&0x00000FFF);
        uint32_t dir_base_addr = this->cr3.flgs.page_dir_base<<12;
        uint32_t dir_addr      = dir_base_addr+(dir_idx<<2);
        this->Error("Not implemented: paging at Cpu::SetCr");
    }
    return linear_addr;
}

void Cpu::UpdateEflagsForInc16(uint16_t result, uint16_t d1, uint16_t d2){
    this->UpdateZF(result);
    this->UpdateSF(result);
    this->UpdatePF(result);
    this->UpdateOF_Add(result, d1, d2);
}

void Cpu::SetR16(SEGMENT_REGISTER register_type, uint16_t data){
    this->segment_registers[register_type]->Set(data, *this);
}

void Cpu::SetGdtr(uint16_t limit, uint32_t base){
    this->gdtr->Set(limit, base);
}

void Cpu::SetIdtr(uint16_t limit, uint32_t base){
    this->idtr->Set(limit, base);
}

void Cpu::SetCr(CONTROL_REGISTER control_register_type, uint32_t data){
    switch(control_register_type){
        case CR0:
            this->cr0.raw = data;
            if(this->cr0.flgs.PG)this->Error("Not implemented: paging at Cpu::SetCr");
            break;
        case CR3:
            this->cr3.raw = data;
            break;
        default:
            this->Error("Not implemented: cr%d at Cpu::SetCr", control_register_type);
    }
}

void Cpu::SetTr(uint16_t selector){
    this->task_register->Set(selector, *this);
}

bool Cpu::Is32bitsMode(){
    return this->segment_registers[CS]->Is32bitsMode();
}

void Cpu::On32bitMode(){
    this->segment_registers[CS]->On32bitMode();
}

uint16_t Cpu::GetR16(SEGMENT_REGISTER register_type){
    return this->segment_registers[register_type]->GetData();
}

uint32_t Cpu::GetCr(CONTROL_REGISTER control_register_type){
    switch(control_register_type){
        case CR0:
            return this->cr0.raw;
        default:
            this->Error("Not implemented: cr%d at Cpu::GetCr", control_register_type);
    }
}

uint32_t Cpu::GetGdtBaseAddr() const{
    return this->gdtr->GetBase();
}

uint32_t Cpu::GetIdtBase(){
    return this->idtr->GetBase();
}

GdtGate* Cpu::GetGdtGate(uint16_t selector) const{
    if((selector-4)%8==0){
        this->Error("Not implement: Ldt at Cpu::GetGdtGate");
    }
    selector = selector&0xFFF8;
    return (GdtGate*)this->mem->GetPointer(selector+this->GetGdtBaseAddr());
}

GdtGate* Cpu::GetLdtGate(uint16_t selector) const{
    return (GdtGate*)this->mem->GetPointer(selector+this->ldtr->GetBaseAddr());
}

IdtGate* Cpu::GetIdtGate(uint16_t selector){
    return (IdtGate*)this->mem->GetPointer(selector+this->GetIdtBase());
}

void Cpu::CallFunctionOnRealMode(uint8_t selector){
    this->bios->CallFunction(*this, *this->mem, selector);
}

uint32_t Cpu::GetLinearAddrForCodeAccess(){
    uint32_t base_addr;
    uint32_t offset;
    
    if(this->cr0.flgs.PE){
        base_addr = this->segment_registers[this->default_code_selector]->GetBaseAddr();
        return this->GetPhysicalAddr(base_addr + this->GetEip());
    }
    base_addr = ((uint32_t)this->segment_registers[CS]->GetData())*16;
    offset    = 0x0000FFFF & this->eip;
    return (base_addr + offset)&0x000FFFFF;//下位20bitがリアルモードにおいてのリニアアドレス
}

uint32_t Cpu::GetLinearAddrForDataAccess(uint32_t offset){
    uint32_t base_addr;
    if(this->cr0.flgs.PE){
        base_addr = this->segment_registers[this->default_data_selector]->GetBaseAddr();
        return this->GetPhysicalAddr(base_addr + offset);
    }
    base_addr = ((uint32_t)this->segment_registers[this->default_data_selector]->GetData())*16;
    offset    = 0x0000FFFF & offset;
    return (base_addr + offset)&0x000FFFFF;//下位20bitがリアルモードにおいてのリニアアドレス
}

uint32_t Cpu::GetLinearStackAddr(){
    uint32_t base_addr;
    uint32_t offset;
    if(this->cr0.flgs.PE){
        offset    = *this->gprs[ESP];
        base_addr = this->segment_registers[this->default_stack_selector]->GetBaseAddr();
        return this->GetPhysicalAddr(offset + base_addr);
    }
    offset    = 0x0000FFFF & *this->gprs[ESP];
    base_addr = ((uint32_t)this->segment_registers[this->default_stack_selector]->GetData())*16;
    return (base_addr + offset)&0x000FFFFF;//下位20bitがリアルモードにおいてのリニアアドレス
}

uint32_t Cpu::GetBaseAddr(SEGMENT_REGISTER register_type){
    if(!this->IsProtectedMode()){//read mode
        return this->segment_registers[register_type]->GetData()*16;
    }
    return this->segment_registers[register_type]->GetBaseAddr();
}

uint16_t Cpu::GetLdtr(){
    return this->ldtr->GetData();
}

uint8_t Cpu::GetCpl(){
    return this->segment_registers[CS]->GetDpl();
}

void Cpu::SetRpl(SEGMENT_REGISTER register_type, uint8_t rpl){
    this->segment_registers[register_type]->SetRpl(rpl);
}

void Cpu::SetCpl(SEGMENT_REGISTER register_type, uint8_t cpl){
    this->segment_registers[register_type]->SetCpl(cpl);
}

Tss* Cpu::GetCurrentTss(){
    return (Tss*)this->mem->GetPointer(this->task_register->GetBaseAddr());
}

bool Cpu::IsSegmentOverride(){
    return this->segment_override;
}

bool Cpu::IsBflg(SEGMENT_REGISTER register_type){//セグメントディスクリプタのBフラグ
    return this->segment_registers[register_type]->Is32bitsMode();
}

inline void Cpu::InitSelector(){
    this->segment_override = false;
    this->default_code_selector = CS;
    this->default_data_selector = DS;
    this->default_stack_selector = SS;
}

inline void Cpu::ResetPrefixFlg(){
    this->prefix_flgs[FLG_67] = false;
    this->prefix_flgs[FLG_66] = false;
    this->prefix_flgs[FLG_F0] = false;
    this->prefix_flgs[FLG_2E] = false;
    this->prefix_flgs[FLG_36] = false;
    this->prefix_flgs[FLG_26] = false;
    this->prefix_flgs[FLG_64] = false;
    this->prefix_flgs[FLG_65] = false;
}

inline void Cpu::CheckPrefixCode(const Memory& mem){
    uint8_t op_code=mem.Read8(this->GetLinearAddrForCodeAccess());
    while(this->prefix_table[op_code]){      
        switch (op_code){
            case FLG_66:
                this->prefix_flgs[FLG_66] = true;
                break;
            case FLG_67:
                this->prefix_flgs[FLG_67] = true;
                break;
            case FLG_2E:
                this->prefix_flgs[FLG_2E] = true;
                this->default_data_selector = CS;
                this->segment_override = true;
                break;
            case FLG_26:
                this->prefix_flgs[FLG_26] = true;
                this->default_data_selector = ES;
                this->segment_override = true;
                break;
            case FLG_36:
                this->prefix_flgs[FLG_36] = true;
                this->default_data_selector = SS;
                this->segment_override = true;
                break;
            case FLG_3E:
                this->prefix_flgs[FLG_3E] = true;
                this->default_data_selector = DS;
                this->segment_override = true;
                break;
            default:
                this->Error("prefix_code : 0x%02X is not implemented at Cpu::CheckPrefixCode\n", op_code);
        }
        this->eip++;
        op_code = mem.Read8(this->GetLinearAddrForCodeAccess());
    }
}

void Cpu::UpdateOF_Sub16(uint16_t result, uint16_t d1, uint16_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG2)!=(d2&SIGN_FLG2)) && ((result&SIGN_FLG2)!=(d1&SIGN_FLG2));
}

void Cpu::UpdateOF_Sub8(uint8_t result, uint8_t d1, uint8_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG1)!=(d2&SIGN_FLG1)) && ((result&SIGN_FLG1)!=(d1&SIGN_FLG1));
}

void Cpu::UpdateOF_Sub(uint32_t result, uint32_t d1, uint32_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG4)!=(d2&SIGN_FLG4)) && ((result&SIGN_FLG4)!=(d1&SIGN_FLG4));
}

void Cpu::UpdateZF(uint32_t result){
    this->eflags.flgs.ZF = (result==0)? 1:0;
}

void Cpu::UpdateCF(uint64_t result){
    this->eflags.flgs.CF = ((result>>32)&1)? 1:0;
}

void Cpu::UpdatePF(uint32_t result){
    int pf_cnt=0;
    for(int i=0; i<BYTE; i++){
        if(((result>>i)&0x01)==0x01){
            pf_cnt++;
        }
    }
    this->eflags.flgs.PF = (pf_cnt%2==0)? 1:0;
}

void Cpu::UpdateEflagsForSub8(uint32_t result, uint8_t d1, uint8_t d2){
    this->UpdateCfForSub(result, 1);
    this->UpdateZF((uint32_t)result);
    this->UpdateOF_Sub8((uint32_t)result, d1, d2);
    this->UpdateSF((uint8_t)result);
    this->UpdatePF(result);
}

void Cpu::UpdateEflagsForSub16(uint32_t result, uint16_t d1, uint16_t d2){
    this->UpdateCfForSub(result, 2);
    this->UpdateZF((uint32_t)result);
    this->UpdateOF_Sub16((uint32_t)result, d1, d2);
    this->UpdateSF((uint16_t)result);
    this->UpdatePF(result);
}

void Cpu::UpdateEflagsForSub(uint64_t result, uint32_t d1, uint32_t d2){
    this->UpdateCfForSub(result, 4);
    this->UpdateZF((uint32_t)result);
    this->UpdateOF_Sub((uint32_t)result, d1, d2);
    this->UpdateSF((uint32_t)result);
    this->UpdatePF(result);
}

void Cpu::UpdateEflagsForInc(uint32_t result, uint32_t d1, uint32_t d2){
    this->UpdateZF(result);
    this->UpdateSF(result);
    this->UpdatePF(result);
    this->UpdateOF_Add(result, d1, d2);
}

void Cpu::UpdateEflagsForInc8(uint8_t result, uint8_t d1, uint8_t d2){
    this->UpdateZF(result);
    this->UpdateSF(result);
    this->UpdatePF(result);
    this->UpdateOF_Add(result, d1, d2);
}

bool Cpu::IsFlag(EFLAGS_KIND eflags_kind){
    switch (eflags_kind){
        case CF:
            return this->eflags.flgs.CF;
        case ZF:
            return this->eflags.flgs.ZF;
        case OF:
            return this->eflags.flgs.OF;
        case SF:
            return this->eflags.flgs.SF;
        case IF:
            return this->eflags.flgs.IF;
        case DF:
            return this->eflags.flgs.DF;
        case PF:
            return this->eflags.flgs.PF;
        default:
            this->Error("Not implemented: %d Cpu::IsFlag", eflags_kind);
    }
}

void Cpu::SetFlag(EFLAGS_KIND eflags_kind){
    switch (eflags_kind){
        case CF:
            this->eflags.flgs.CF = 1;
            return;
        case IF:
            this->eflags.flgs.IF = 1;
            return;
        case OF:
            this->eflags.flgs.OF = 1;
            return;
        case DF:
            this->eflags.flgs.DF = 1;
            return;
        case ZF:
            this->eflags.flgs.ZF = 1;
            return;
        default:
            this->Error("Not implemented: %d Cpu::SetFlag", eflags_kind);
    }
}

void Cpu::ClearFlag(EFLAGS_KIND eflags_kind){
    switch (eflags_kind){
        case CF:
            this->eflags.flgs.CF = 0;
            return;
        case IF:
            this->eflags.flgs.IF = 0;
            return;
        case OF:
            this->eflags.flgs.OF = 0;
            return;
        case DF:
            this->eflags.flgs.DF = 0;
            return;
        case TF:
            this->eflags.flgs.TF = 0;
            return;
        case AC:
            this->eflags.flgs.AC = 0;
            return;
        case ZF:
            this->eflags.flgs.ZF = 0;
            break;
        default:
            this->Error("Not implemented: %d Cpu::ClearFlag", eflags_kind);
    }
}

inline void Cpu::Push16(uint16_t data){
    *this->gprs[ESP] = *this->gprs[ESP]-2;
    uint32_t addr   = this->GetLinearStackAddr();
    this->mem->Write(addr, data);
}


inline void Cpu::Push32(uint32_t data){
    *this->gprs[ESP] = *this->gprs[ESP]-4;
    uint32_t addr   = this->GetLinearStackAddr();
    this->mem->Write(addr, data);
}

void Cpu::SaveTask(uint16_t selector){
    uint16_t prev_selector;
    prev_selector = this->task_register->GetData();
    this->task_register->Set(selector, *this);
    GdtGate*gdt_gate = this->GetGdtGate(prev_selector);
    uint32_t task_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;
    this->mem->Write(task_addr+8*4, this->GetEip());
    this->mem->Write(task_addr+9*4, this->GetEflgs());
    this->mem->Write(task_addr+10*4, this->registers);
    this->mem->Write(task_addr+18*4, this->GetR16(ES));
    this->mem->Write(task_addr+19*4, this->GetR16(CS));
    this->mem->Write(task_addr+20*4, this->GetR16(SS));
    this->mem->Write(task_addr+21*4, this->GetR16(DS));
    this->mem->Write(task_addr+22*4, this->GetR16(FS));
    this->mem->Write(task_addr+23*4, this->GetR16(GS));
    this->mem->Write(task_addr+24*4, this->ldtr->GetData());
}

void Cpu::SetLdtr(uint16_t data){
    this->ldtr->Set(data, *this);
}

void Cpu::SwitchTask(){
    GdtGate* gdt_gate = this->GetGdtGate(this->task_register->GetData());
    uint32_t task_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;

    this->SetEip(this->mem->Read32(task_addr+8*4));
    this->SetEflgs(this->mem->Read32(task_addr+9*4));
    memcpy(&this->registers, this->mem->GetPointer(task_addr+10*4), sizeof(Registers));
    this->SetR16(ES, this->mem->Read16(task_addr+18*4));
    this->SetR16(CS, this->mem->Read16(task_addr+19*4));
    this->SetR16(SS, this->mem->Read16(task_addr+20*4));
    this->SetR16(DS, this->mem->Read16(task_addr+21*4));
    this->SetR16(FS, this->mem->Read16(task_addr+22*4));
    this->SetR16(GS, this->mem->Read16(task_addr+23*4));
    this->ldtr->Set(this->mem->Read16(task_addr+24*4), *this);
}

void Cpu::HandleInterrupt(int irq_num){
    if(!this->IsProtectedMode()){
        this->Push16(this->eflags.raw);
        this->Push16(this->segment_registers[CS]->GetData());
        this->Push16(this->eip);
        this->eip   = this->mem->Read16(irq_num*4);
        this->SetR16(CS, this->mem->Read16(irq_num*4+2));
        this->is_exception_ = false;
        return;
    }
    //TODO : 変数の宣言のスコープを縮める。
    irq_num = irq_num + 0x20;
    uint16_t selector, cs, ss;
    uint32_t eip, eflags, esp;
    uint32_t offset_addr;
    uint8_t cpl, dest_code_segment_dpl;
    Tss* tss;
    IdtGate* idt_gate = (IdtGate*)this->mem->GetPointer(this->idtr->GetBase()+irq_num*8);
    GdtGate* gdt_gate = this->GetGdtGate(idt_gate->selector);
    dest_code_segment_dpl = CpuHelper::GetDpl(gdt_gate->access_right);
    cpl = this->GetCpl();
    offset_addr = (((uint32_t)idt_gate->offset_high)<<16) | ((uint32_t)idt_gate->offset_low);
    if(dest_code_segment_dpl==cpl){
        this->Push32(this->eflags.raw);
        this->Push32(this->segment_registers[CS]->GetData());
        this->Push32(this->eip);
        this->eip = offset_addr;
        this->segment_registers[CS]->Set(idt_gate->selector, *this);
        this->eflags.flgs.IF = 0;
    }else if(dest_code_segment_dpl<cpl){
        ss = this->GetR16(SS);
        eflags = this->GetEflgs();
        esp = this->GetR32(ESP);
        cs = this->GetR16(CS);
        eip = this->GetEip();
        tss = this->GetCurrentTss();
        this->SetR16(SS, tss->ss0);
        this->SetR32(ESP, tss->esp0);
        this->Push32(ss);
        this->Push32(esp);
        this->Push32(eflags);
        this->Push32(cs);
        this->Push32(eip);
        this->SetEip(offset_addr);
        this->SetR16(CS, idt_gate->selector);
        //this->SetRpl(CS, cpl);
        this->eflags.flgs.IF = 0;
    }else{
        this->Error("Not implemented: dest_code_segment_dpl>cpl at Cpu::HandleInterrupt");
    }

    return;
}

//#define OFFSET 10
#define OFFSET 35

static void Dump(const Memory& mem, uint32_t start, uint32_t size){
    for(int i=0; i<size; i++){
        if(i%8==0){
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "%02X ", mem.Read8(start+i));
    }
    fprintf(stderr, "\n");
}

void Cpu::ShowRegisters(){
    fprintf(stderr, "EAX=%08X EBX=%08X ECX=%08X EDX=%08X\n", *this->gprs[EAX], *this->gprs[EBX], *this->gprs[ECX], *this->gprs[EDX]);
    fprintf(stderr, "ESI=%08X EDI=%08X EBP=%08X ESP=%08X\n", *this->gprs[ESI], *this->gprs[EDI], *this->gprs[EBP], *this->gprs[ESP]);
    fprintf(stderr, "EIP=%08X\n", this->eip);
    fprintf(stderr, "ES =%04X %08X\n", this->segment_registers[ES]->GetData(), this->segment_registers[ES]->GetBaseAddr());
    fprintf(stderr, "CS =%04X %08X\n", this->segment_registers[CS]->GetData(), this->segment_registers[CS]->GetBaseAddr());
    fprintf(stderr, "SS =%04X %08X\n", this->segment_registers[SS]->GetData(), this->segment_registers[SS]->GetBaseAddr());
    fprintf(stderr, "DS =%04X %08X\n", this->segment_registers[DS]->GetData(), this->segment_registers[DS]->GetBaseAddr());
    fprintf(stderr, "FS =%04X %08X\n", this->segment_registers[FS]->GetData(), this->segment_registers[FS]->GetBaseAddr());
    fprintf(stderr, "GS =%04X %08X\n", this->segment_registers[GS]->GetData(), this->segment_registers[GS]->GetBaseAddr());
    fprintf(stderr, "LDT=%04X %08X\n", this->ldtr->GetData(), this->ldtr->GetBaseAddr());
    fprintf(stderr, "TR =%04X %08X\n", this->task_register->GetData(), this->task_register->GetBaseAddr());
    fprintf(stderr, "GDT=%08X\n", this->gdtr->GetBase());
    fprintf(stderr, "IDT=%08X\n", this->idtr->GetBase());
    fprintf(stderr, "CR0=%08X\n", this->cr0.raw);
    fprintf(stderr, "MODE = %s\n", this->IsProtectedMode()?"PROTECTED" : "REAL");
}

//TODO : bool引数はenumに変更すべき
void Cpu::Debug(FILE *f, bool h) {
    //if (h) fprintf(f, " AX   BX   CX   DX   SP   BP   SI   DI    FLAGS    ES   SS   DS   CS   IP  dump\n");
    if (h) fprintf(f, " AX   BX   CX   DX   SP   BP   SI   DI   ES   SS   DS   CS   IP   P(EFLAGS) S(EFLAGS)\n");
        fprintf(f,
            "%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %c %c\n",
            this->GetR16(EAX), this->GetR16(EBX), this->GetR16(ECX), this->GetR16(EDX), this->GetR16(ESP), this->GetR16(EBP), this->GetR16(ESI), this->GetR16(EDI),
            this->segment_registers[ES]->GetData(), this->segment_registers[SS]->GetData(), this->segment_registers[DS]->GetData(), this->segment_registers[CS]->GetData(), this->eip, this->eflags.flgs.PF?'P':'-', this->eflags.flgs.SF?'S':'-');
}

bool Cpu::Run(const Emulator& emu){
    static vector<uint32_t> eip_history;
    static long long cnt=0;
    cnt++;
    try{//TODO: エラー処理はtry catchで処理するようにする。まだ未実装の箇所が多い。
        eip_history.push_back(this->eip);
        //if(this->eip==0xD58F){
        if(cnt==790738){
            cnt = cnt;
        }
        if(this->eip==0x0000D58F){
            for(int i=eip_history.size()-100; i<eip_history.size(); i++){
                fprintf(stderr, "EIP=0x%08X\n", eip_history[i]);
            }
            fprintf(stderr, "cnt=%d\n", cnt);
            throw "\n";
        }
        if(this->eip==0x0000220a){
            int i=0;
            i++;
        }
        this->InitSelector();
        this->ResetPrefixFlg();
        this->CheckPrefixCode(*(this->mem));
        uint8_t op_code = this->mem->Read8(this->GetLinearAddrForCodeAccess());
        if(this->instructions[op_code].get()==NULL){
            //Dump(this->mem, this->GetLinearAddrForCodeAccess(), 512);
            this->Error("Not implemented: op_code = 0x%02X Cpu::Run\n", op_code);
        }
        this->instructions[op_code]->Run(emu);
        //fprintf(stderr, "%s\n", this->instructions[op_code]->code_name.c_str());
        return true;
    }catch(const char* error_message){
        cerr << error_message << endl;
        this->ShowRegisters();
        return false;
    }
}
