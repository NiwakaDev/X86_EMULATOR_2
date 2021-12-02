#include "Cpu.h"
#include "SegmentRegister.h"
#include "Memory.h"
#include "Bios.h"
//#include "Instruction.h"
#include "Gdtr.h"
#include "Idtr.h"
#include "Ldtr.h"
#include "TaskRegister.h"

using namespace std;

#define SIGN_FLG 0x80000000//いずれ消す予定
#define BYTE 8

Cpu::Cpu(Bios* bios, Memory* mem){
    this->bios = bios;
    this->mem  = mem;
    this->eflags.raw = 0x00000002;
    this->cr0.raw = 0;
    this->cr0.flgs.PE  = REAL_MODE;
    for(int i=0; i<SEGMENT_REGISTER_COUNT; i++){
        this->segment_registers[i] = new SegmentRegister(0x0000);
    }
    this->task_register = new TaskRegister(0x00);

    this->ldtr = new Ldtr(0);
    this->gdtr = new Gdtr("Gdtr", 0, 0);
    this->idtr = new Idtr("Idtr", 0, 0);
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->gprs[i] = 0x00000000;
    }

    this->prefix_flgs[FLG_67] = false;
    this->prefix_flgs[FLG_66] = false;
    this->prefix_flgs[FLG_F0] = false;
    this->prefix_flgs[FLG_F2] = false;
    this->prefix_flgs[FLG_F3] = false;
    this->prefix_flgs[FLG_2E] = false;
    this->prefix_flgs[FLG_36] = false;
    this->prefix_flgs[FLG_26] = false;
    this->prefix_flgs[FLG_64] = false;
    this->prefix_flgs[FLG_65] = false;


    this->prefix_table[0xF0] = true;
    this->prefix_table[0xF2] = true;
    this->prefix_table[0xF3] = true;
    this->prefix_table[0x26] = true;
    this->prefix_table[0x2E] = true;
    this->prefix_table[0x36] = true;
    this->prefix_table[0x3E] = true;
    this->prefix_table[0x64] = true;
    this->prefix_table[0x65] = true;
    this->prefix_table[0x66] = true;
    this->prefix_table[0x67] = true;
    for(int i=0; i<INSTRUCTION_SIZE; i++){
        this->instructions[i] = NULL;
    }
    //this->instructions[0x00] = new AddRm8R8("AddRm8R8");
    this->instructions[0x01] = new AddRm32R32("AddRm32R32");
    this->instructions[0x02] = new AddR8Rm8("AddR8Rm8");
    this->instructions[0x03] = new AddR32Rm32("AddR32Rm32");
    this->instructions[0x05] = new AddEaxImm32("AddEaxImm32");
    this->instructions[0x06] = new PushEs("PushEs");
    this->instructions[0x07] = new PopEs("PopEs");
    this->instructions[0x09] = new OrRm32R32("OrRm32R32");
    this->instructions[0x10] = new AdcRm8R8("AdcRm8R8");
    this->instructions[0x11] = new AdcRm32R32("AdcRm32R32");
    this->instructions[0x0D] = new OrEaxImm32("OrEaxImm32");
    this->instructions[0x0F] = new Code0F("Code0F");
    this->instructions[0x16] = new PushSs("PushSs");
    this->instructions[0x18] = new SbbRm8R8("SbbRm8R8");
    this->instructions[0x1A] = new SbbR8Rm8("SbbR8Rm8");
    this->instructions[0x1E] = new PushDs("PushDs");
    this->instructions[0x1F] = new PopDs("PopDs");
    this->instructions[0x20] = new AndRm8R8("AndRm8R8");
    this->instructions[0x21] = new AndRm32R32("AndRm32R32");
    this->instructions[0x22] = new AndR8Rm8("AndR8Rm8");
    this->instructions[0x23] = new AndR32Rm32("AndR32Rm32");
    this->instructions[0x24] = new AndAlImm8("AndAlImm8");
    this->instructions[0x25] = new AndEaxImm32("AndEaxImm32");
    this->instructions[0x28] = new SubRm8R8("SubRm8R8");
    this->instructions[0x29] = new SubRm32R32("SubRm32R32");
    this->instructions[0x2B] = new SubR32Rm32("SubR32Rm32");
    this->instructions[0x2D] = new SubEaxImm32("SubEaxImm32");
    this->instructions[0x31] = new XorRm32R32("XorRm32R32");
    this->instructions[0x38] = new CmpRm8R8("CmpRm8R8");
    this->instructions[0x39] = new CmpRm32R32("CmpRm32R32");
    this->instructions[0x3A] = new CmpR8Rm8("CmpR8Rm8");
    this->instructions[0x3B] = new CmpR32Rm32("CmpR32Rm32");
    this->instructions[0x3C] = new CmpAlImm8("CmpAlImm8");
    this->instructions[0x3D] = new CmpEaxImm32("CmpEaxImm32");
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0x40+i] = new IncR32("IncR32");
    }
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0x48+i] = new DecR32("DecR32");
    }
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0x50+i] = new PushR32("PushR32");
    }
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0x58+i] = new PopR32("PopR32");
    }
    this->instructions[0x60] = new PushAd("PushAd");
    this->instructions[0x61] = new PopAd("PopAd");
    this->instructions[0x68] = new PushImm32("PushImm32");
    this->instructions[0x69] = new ImulR32Rm32Imm32("ImulR32Rm32Imm32");
    this->instructions[0x6A] = new PushImm8("PushImm8");
    this->instructions[0x6B] = new ImulR32Rm32Imm8("ImulR32Rm32Imm8");
    this->instructions[0x72] = new JcRel8("JcRel8");
    this->instructions[0x73] = new JaeRel8("JaeRel8");
    this->instructions[0x74] = new JzRel8("JzRel8");
    this->instructions[0x75] = new JnzRel8("JnzRel8");
    this->instructions[0x76] = new JbeRel8("JbeRel8");
    this->instructions[0x77] = new JaRel8("JaRel8");
    this->instructions[0x7C] = new JlRel8("JlRel8");
    this->instructions[0x7E] = new JleRel8("JleRel8");
    this->instructions[0x7F] = new JgRel8("JgRel8");
    this->instructions[0x78] = new JsRel8("JsRel8");
    this->instructions[0x79] = new JnsRel8("JnsRel8");
    this->instructions[0x7D] = new JgeRel8("JgeRel8");
    this->instructions[0x80] = new Code80("Code80");
    this->instructions[0x81] = new Code81("Code81");
    this->instructions[0x83] = new Code83("Code83");
    this->instructions[0x84] = new TestRm8R8("TestRm8R8");
    this->instructions[0x85] = new TestRm32R32("TestRm32R32");
    this->instructions[0x87] = new XchgR32Rm32("XchgR32Rm32");
    this->instructions[0x88] = new MovRm8R8("MovRm8R8");
    this->instructions[0x89] = new MovRm32R32("MovRm32R32");
    this->instructions[0x8A] = new MovR8Rm8("MovR8Rm8");
    this->instructions[0x8B] = new MovR32Rm32("MovR32Rm32");
    this->instructions[0x8C] = new MovRm16Sreg("MovRm16Sreg");
    this->instructions[0x8D] = new LeaR32M("LeaR32M");
    this->instructions[0x8E] = new MovSregRm16("MovSregRm16");
    //this->instructions[0x8F] = new PopM32("PopM32");
    this->instructions[0x90] = new Nop("Nop");
    this->instructions[0x98] = new Cwde("Cwde");
    this->instructions[0x99] = new Cdq("Cdq");
    this->instructions[0x9A] = new CallPtr1632("CallPtr1632");
    this->instructions[0x9C] = new PushFd("PushFd");
    this->instructions[0x9D] = new PopFd("PopFd");
    this->instructions[0xA0] = new MovAlMoffs8("MovAlMoffs8");
    this->instructions[0xA1] = new MovEaxMoffs32("MovEaxMoffs32");
    this->instructions[0xA2] = new MovMoffs8Al("MovMoffs8Al");
    this->instructions[0xA3] = new MovMoffs32Eax("MovMoffs32Eax");
    this->instructions[0xA5] = new MovM32M32("MovM32M32");
    this->instructions[0xA6] = new CmpsM8M8("CmpsM8M8");
    this->instructions[0xA9] = new TestEaxImm32("TestEaxImm32");
    this->instructions[0xAC] = new LodsM8("LodsM8");
    this->instructions[0xAE] = new Scasb("Scasb");
    this->instructions[0xC0] = new CodeC0("CodeC0");
    this->instructions[0xC1] = new CodeC1("CodeC1");
    this->instructions[0xC3] = new Ret32Near("Ret32Near");
    this->instructions[0xC6] = new CodeC6("CodeC6");
    this->instructions[0xC7] = new MovRm32Imm32("MovRm32Imm32");
    this->instructions[0xC9] = new Leave("Leave");
    this->instructions[0xCB] = new Ret32Far("Ret32Far");
    this->instructions[0xCD] = new IntImm8("IntImm8");
    this->instructions[0xCF] = new Iretd("Iretd");
    this->instructions[0xD1] = new CodeD1("CodeD1");
    this->instructions[0xD2] = new CodeD2("CodeD2");
    this->instructions[0xD3] = new CodeD3("CodeD3");
    this->instructions[0xD7] = new Xlatb("Xlatb");
    this->instructions[0xE4] = new InAlImm8("InAlImm8");
    this->instructions[0xE6] = new OutImm8("OutImm8");
    this->instructions[0xE8] = new CallRel32("CallRel32");
    this->instructions[0xE9] = new JmpRel32("JmpRel32");
    this->instructions[0xEA] = new JmpPtr1632("JmpPtr1632");
    this->instructions[0xEB] = new JmpRel8("JmpRel8");
    this->instructions[0xEC] = new InAlDx("InAlDx");
    this->instructions[0xEE] = new OutDxAl("OutDxAl");
    this->instructions[0xF4] = new Hlt("Hlt");
    this->instructions[0xF6] = new CodeF6("CodeF6");
    this->instructions[0xF7] = new CodeF7("CodeF7");
    this->instructions[0xF8] = new Clc("Clc");
    this->instructions[0xFA] = new Cli("Cli");
    this->instructions[0xFB] = new Sti("Sti");
    this->instructions[0xFC] = new Cld("Cld");
    this->instructions[0xFE] = new CodeFE("CodeFE");
    this->instructions[0xFF] = new CodeFF("CodeFF");
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0xB0+i] = new MovR8Imm8("MovR8Imm8");
    }
    for(int i=0; i<GENERAL_PURPOSE_REGISTER32_COUNT; i++){
        this->instructions[0xB8+i] = new MovR32Imm32("MovR32Imm32");
    }
}

void Cpu::SetR16(SEGMENT_REGISTER register_type, uint16_t data){
    if(SS==register_type&&data==0x35){
        fprintf(stderr, "cc");
    }
    this->segment_registers[register_type]->Set(data, this);
}

void Cpu::SetR8L(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data){
    this->gprs[register_type] = (this->gprs[register_type]&0xffffff00)|(uint32_t)data;
}

void Cpu::SetR8H(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data){
    this->gprs[register_type] = (this->gprs[register_type]&0xffff00FF)|((uint32_t)data<<8);
}

void Cpu::SetR8(uint32_t register_type, uint8_t data){
    if(register_type<4){
        this->SetR8L((GENERAL_PURPOSE_REGISTER32)register_type, data);
    }else{
        this->SetR8H((GENERAL_PURPOSE_REGISTER32)(register_type-4), data);
    }
}

void Cpu::SetR16(GENERAL_PURPOSE_REGISTER32 register_type, uint16_t data){
    this->gprs[register_type] = (this->gprs[register_type]&0xffff0000)|(uint32_t)data;
}

void Cpu::SetR32(GENERAL_PURPOSE_REGISTER32 register_type, uint32_t data){
    this->gprs[register_type] = data;
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
            break;
        default:
            this->Error("Not implemented: cr%d at Cpu::SetCr", control_register_type);
    }
}

void Cpu::SetTr(uint16_t selector){
    this->task_register->Set(selector, this);
}

void Cpu::SetDataSelector(SEGMENT_REGISTER register_type){
    this->default_data_selector = register_type;
}

void Cpu::SetEflgs(uint32_t eflgs){
    this->eflags.raw = eflgs;
}

bool Cpu::IsPrefixAddrSize(){
    return this->prefix_flgs[FLG_67];
}

bool Cpu::IsPrefixOpSize(){
    return this->prefix_flgs[FLG_66];
}

bool Cpu::IsPrefixRep(){
    return this->prefix_flgs[FLG_F3];
}

bool Cpu::IsPrefixRepnz(){
    return this->prefix_flgs[FLG_F2];
}

bool Cpu::IsProtectedMode(){
    return (bool)this->cr0.flgs.PE;
}

bool Cpu::Is32bitsMode(){
    return this->segment_registers[CS]->Is32bitsMode();
}

void Cpu::On32bitMode(){
    this->segment_registers[CS]->On32bitMode();
}

uint8_t Cpu::GetR8H(GENERAL_PURPOSE_REGISTER32 register_type){
    return (uint8_t)((this->gprs[register_type]>>8)&0xff);
}

uint8_t Cpu::GetR8L(GENERAL_PURPOSE_REGISTER32 register_type){
    return (uint8_t)(this->gprs[register_type]&0x000000ff);
}

uint8_t Cpu::GetR8(uint32_t register_type){
    if(register_type<4){
        return this->GetR8L((GENERAL_PURPOSE_REGISTER32)register_type);
    }else{
        return this->GetR8H((GENERAL_PURPOSE_REGISTER32)(register_type-4));
    }
}

uint16_t Cpu::GetR16(SEGMENT_REGISTER register_type){
    return this->segment_registers[register_type]->GetData();
}

uint16_t Cpu::GetR16(GENERAL_PURPOSE_REGISTER32 register_type){
    return this->gprs[register_type]&0x0000FFFF;
}

uint32_t Cpu::GetR32(GENERAL_PURPOSE_REGISTER32 register_type){
    return this->gprs[register_type];
}

uint32_t Cpu::GetEip(){
    return this->eip;
}

uint16_t Cpu::GetIp(){
    return this->eip&0x0000FFFF;
}

uint32_t Cpu::GetEflgs(){
    return this->eflags.raw;
}

uint32_t Cpu::GetCr(CONTROL_REGISTER control_register_type){
    switch(control_register_type){
        case CR0:
            return this->cr0.raw;
        default:
            this->Error("Not implemented: cr%d at Cpu::GetCr", control_register_type);
    }
}

uint32_t Cpu::GetGdtBaseAddr(){
    return this->gdtr->GetBase();
}

uint32_t Cpu::GetIdtBase(){
    return this->idtr->GetBase();
}

GdtGate* Cpu::GetGdtGate(uint16_t selector){
    if((selector-4)%8==0){
        this->Error("Not implement: Ldt at Cpu::GetGdtGate");
    }
    selector = selector&0xFFF8;
    return (GdtGate*)this->mem->GetPointer(selector+this->GetGdtBaseAddr());
}

GdtGate* Cpu::GetLdtGate(uint16_t selector){
    return (GdtGate*)this->mem->GetPointer(selector+this->ldtr->GetBaseAddr());
}

IdtGate* Cpu::GetIdtGate(uint16_t selector){
    return (IdtGate*)this->mem->GetPointer(selector+this->GetIdtBase());
}

void Cpu::CallFunctionOnRealMode(Memory* mem, uint8_t selector){
    this->bios->CallFunction(this, mem, selector);
}

uint32_t Cpu::GetLinearAddrForCodeAccess(){
    uint32_t base_addr;
    uint32_t offset;
    
    if(this->cr0.flgs.PE){
        base_addr = this->segment_registers[this->default_code_selector]->GetBaseAddr();
        return base_addr + this->GetEip();
    }
    base_addr = ((uint32_t)this->segment_registers[CS]->GetData())*16;
    offset    = 0x0000FFFF & this->eip;
    return base_addr + offset;
}

uint32_t Cpu::GetLinearAddrForDataAccess(uint32_t offset){
    uint32_t base_addr;
    if(this->cr0.flgs.PE){
        base_addr = this->segment_registers[this->default_data_selector]->GetBaseAddr();
        return base_addr + offset;
    }
    base_addr = ((uint32_t)this->segment_registers[this->default_data_selector]->GetData())*16;
    offset    = 0x0000FFFF & offset;
    return base_addr + offset;
}

uint32_t Cpu::GetLinearStackAddr(){
    uint32_t base_addr;
    uint32_t offset;
    if(this->cr0.flgs.PE){
        offset    = this->gprs[ESP];
        base_addr = this->segment_registers[this->default_stack_selector]->GetBaseAddr();
        return offset + base_addr;
    }else{
        if(this->Is32bitsMode() ^ this->IsPrefixAddrSize()){
            offset    = this->gprs[ESP];
        }else{
            offset = (uint32_t)this->GetR16(ESP);
            offset    = 0x0000FFFF & offset;
        }
        base_addr = ((uint32_t)this->segment_registers[this->default_stack_selector]->GetData())*16;
        offset    = 0x0000FFFF & offset;
    }
    return base_addr + offset;
}

uint32_t Cpu::GetBaseAddr(SEGMENT_REGISTER register_type){
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

void Cpu::InitSelector(){
    this->default_code_selector = CS;
    this->default_data_selector = DS;
    this->default_stack_selector = SS;
}

void Cpu::AddEip(uint32_t data){
    this->eip += data;
}

void Cpu::AddIp(uint16_t data){
    this->eip = (this->eip+data)&0x0000FFFF;
}

void Cpu::SetEip(uint32_t addr){
    this->eip = addr;
}

void Cpu::ResetPrefixFlg(){
    this->prefix_flgs[FLG_67] = false;
    this->prefix_flgs[FLG_66] = false;
    this->prefix_flgs[FLG_F0] = false;
    this->prefix_flgs[FLG_F2] = false;
    this->prefix_flgs[FLG_F3] = false;
    this->prefix_flgs[FLG_2E] = false;
    this->prefix_flgs[FLG_36] = false;
    this->prefix_flgs[FLG_26] = false;
    this->prefix_flgs[FLG_64] = false;
    this->prefix_flgs[FLG_65] = false;
}

void Cpu::CheckPrefixCode(Memory* mem){
    uint8_t op_code=mem->Read8(this->GetLinearAddrForCodeAccess());
    while(this->prefix_table[op_code]){      
        switch (op_code){
            case FLG_66:
                this->prefix_flgs[FLG_66] = true;
                break;
            case FLG_67:
                this->prefix_flgs[FLG_67] = true;
                break;
            case FLG_F2:
                this->prefix_flgs[FLG_F2] = true;
                break;
            case FLG_F3:
                this->prefix_flgs[FLG_F3] = true;
                break;
            case FLG_2E:
                this->prefix_flgs[FLG_2E] = true;
                this->default_data_selector = CS;
                break;
            case FLG_26:
                this->prefix_flgs[FLG_26] = true;
                this->default_data_selector = ES;
                break;
            //case FLG_65:
                //this->prefix_flgs[FLG_65] = true;
                //this->default_data_selector = GS;
                //break;
            default:
                this->Error("prefix_code : 0x%02X is not implemented at Cpu::CheckPrefixCode\n", op_code);
        }
        this->eip++;
        op_code = mem->Read8(this->GetLinearAddrForCodeAccess());
    }
}

void Cpu::UpdateOF_Sub16(uint16_t result, uint16_t d1, uint16_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG2)!=(d2&SIGN_FLG2)) && ((result&SIGN_FLG2)!=(d1&SIGN_FLG2));
}

void Cpu::UpdateOF_Sub8(uint8_t result, uint8_t d1, uint8_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG1)!=(d2&SIGN_FLG1)) && ((result&SIGN_FLG1)!=(d1&SIGN_FLG1));
}

void Cpu::UpdateOF_Sub(uint32_t result, uint32_t d1, uint32_t d2){
    this->eflags.flgs.OF = ((d1&SIGN_FLG)!=(d2&SIGN_FLG)) && ((result&SIGN_FLG)!=(d1&SIGN_FLG));
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
}

void Cpu::UpdateEflagsForSub16(uint32_t result, uint16_t d1, uint16_t d2){
    this->UpdateCfForSub(result, 2);
    this->UpdateZF((uint32_t)result);
    this->UpdateOF_Sub16((uint32_t)result, d1, d2);
    this->UpdateSF((uint16_t)result);
}

void Cpu::UpdateEflagsForSub(uint64_t result, uint32_t d1, uint32_t d2){
    this->UpdateCfForSub(result, 4);
    this->UpdateZF((uint32_t)result);
    this->UpdateOF_Sub((uint32_t)result, d1, d2);
    this->UpdateSF((uint32_t)result);
}

void Cpu::UpdateEflagsForInc(uint32_t result, uint32_t d1, uint32_t d2){
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
        default:
            this->Error("Not implemented: %d Cpu::ClearFlag", eflags_kind);
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
        default:
            this->Error("Not implemented: %d Cpu::ClearFlag", eflags_kind);
    }
}

void Cpu::Push32(uint32_t data){
    this->gprs[ESP] = this->gprs[ESP]-4;
    uint32_t addr   = this->GetLinearStackAddr();
    this->mem->Write(addr, data);
}

void Cpu::SaveTask(uint16_t selector){
    uint32_t task_addr;
    uint16_t prev_selector;
    prev_selector = this->task_register->GetData();
    this->task_register->Set(selector, this);
    GdtGate*gdt_gate = this->GetGdtGate(prev_selector);
    task_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;
    //this->mem->Write(task_addr+1*4, this->GetEip());
    //this->mem->Write(task_addr+2*4, this->GetR16(SS));
    this->mem->Write(task_addr+8*4, this->GetEip());
    this->mem->Write(task_addr+9*4, this->GetEflgs());
    this->mem->Write(task_addr+10*4, this->GetR32(EAX));
    this->mem->Write(task_addr+11*4, this->GetR32(ECX));
    this->mem->Write(task_addr+12*4, this->GetR32(EDX));
    this->mem->Write(task_addr+13*4, this->GetR32(EBX));
    this->mem->Write(task_addr+14*4, this->GetR32(ESP));
    this->mem->Write(task_addr+15*4, this->GetR32(EBP));
    this->mem->Write(task_addr+16*4, this->GetR32(ESI));
    this->mem->Write(task_addr+17*4, this->GetR32(EDI));
    this->mem->Write(task_addr+18*4, this->GetR16(ES));
    this->mem->Write(task_addr+19*4, this->GetR16(CS));
    this->mem->Write(task_addr+20*4, this->GetR16(SS));
    this->mem->Write(task_addr+21*4, this->GetR16(DS));
    this->mem->Write(task_addr+22*4, this->GetR16(FS));
    this->mem->Write(task_addr+23*4, this->GetR16(GS));
    this->mem->Write(task_addr+24*4, this->ldtr->GetData());
}

void Cpu::SwitchTask(){
    uint32_t task_addr;
    GdtGate* gdt_gate = this->GetGdtGate(this->task_register->GetData());

    task_addr = (((uint32_t)gdt_gate->base_high)<<24) | (((uint32_t)gdt_gate->base_mid)<<16) | (uint32_t)gdt_gate->base_low;

    this->SetEip(this->mem->Read32(task_addr+8*4));
    this->SetEflgs(this->mem->Read32(task_addr+9*4));
    this->SetR32(EAX, this->mem->Read32(task_addr+10*4));
    this->SetR32(ECX, this->mem->Read32(task_addr+11*4));
    this->SetR32(EDX, this->mem->Read32(task_addr+12*4));
    this->SetR32(EBX, this->mem->Read32(task_addr+13*4));
    this->SetR32(ESP, this->mem->Read32(task_addr+14*4));
    this->SetR32(EBP, this->mem->Read32(task_addr+15*4));
    this->SetR32(ESI, this->mem->Read32(task_addr+16*4)); 
    this->SetR32(EDI, this->mem->Read32(task_addr+17*4)); 
    this->SetR16(ES, this->mem->Read16(task_addr+18*4));
    this->SetR16(CS, this->mem->Read16(task_addr+19*4));
    this->SetR16(SS, this->mem->Read16(task_addr+20*4));
    this->SetR16(DS, this->mem->Read16(task_addr+21*4));
    this->SetR16(FS, this->mem->Read16(task_addr+22*4));
    this->SetR16(GS, this->mem->Read16(task_addr+23*4));
    this->ldtr->Set(this->mem->Read16(task_addr+24*4), this);
}

void Cpu::HandleInterrupt(int irq_num){
    irq_num = irq_num + 0x20;
    uint16_t selector, cs, ss;
    uint32_t eip, eflags, esp;
    uint32_t offset_addr;
    uint8_t cpl, dest_code_segment_dpl;
    Tss* tss;
    IdtGate* idt_gate = (IdtGate*)this->mem->GetPointer(this->idtr->GetBase()+irq_num*8);
    GdtGate* gdt_gate = this->GetGdtGate(idt_gate->selector);
    dest_code_segment_dpl = GET_DPL(gdt_gate->access_right);
    cpl = this->GetCpl();
    offset_addr = (((uint32_t)idt_gate->offset_high)<<16) | ((uint32_t)idt_gate->offset_low);
    if(dest_code_segment_dpl==cpl){
        this->Push32(this->eflags.raw);
        this->Push32(this->segment_registers[CS]->GetData());
        this->Push32(this->eip);
        this->eip = offset_addr;
        this->segment_registers[CS]->Set(idt_gate->selector, this);
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

    /***
    ss = this->segment_registers[SS]->GetData();
    eflags = this->eflags.raw;
    esp = this->gprs[ESP];
    cs  = this->segment_registers[CS]->GetData();
    eip = this->eip;
    tss = this->GetCurrentTss();
    this->SetR16(SS, tss->ss0);
    this->SetR32(ESP, tss->esp0);
    this->Push32(ss);
    this->Push32(esp);
    this->Push32(eflags);
    this->Push32(cs);
    this->Push32(eip);
    this->eip = offset_addr;
    this->segment_registers[CS]->Set(idt_gate->selector,  this);
    this->eflags.flgs.IF = 0;
    ***/
    return;
}

//#define OFFSET 10
#define OFFSET 35

static void Dump(Memory* mem, uint32_t start, uint32_t size){
    for(int i=0; i<size; i++){
        if(i%8==0){
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "%02X ", mem->Read8(start+i));
    }
    fprintf(stderr, "\n");
}

void Cpu::ShowRegisters(){
    fprintf(stderr, "EAX=%08X EBX=%08X ECX=%08X EDX=%08X\n", this->gprs[EAX], this->gprs[EBX], this->gprs[ECX], this->gprs[EDX]);
    fprintf(stderr, "ESI=%08X EDI=%08X EBP=%08X ESP=%08X\n", this->gprs[ESI], this->gprs[EDI], this->gprs[EBP], this->gprs[ESP]);
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
    fprintf(stderr, "GDT=%08X\n", this->idtr->GetBase());
}

void Cpu::Run(IoPort* io_port){
    this->InitSelector();
    this->ResetPrefixFlg();
    this->CheckPrefixCode(this->mem);
    uint8_t op_code = this->mem->Read8(this->GetLinearAddrForCodeAccess());
    if(this->instructions[op_code]==NULL){
        this->Error("Not implemented: op_code = 0x%02X Cpu::Run", op_code);
    }
    this->instructions[op_code]->Run(this, this->mem, io_port);
    //fprintf(stderr, "%s\n", this->instructions[op_code]->code_name.c_str());
    return;
}
