#include "../Cpu.h"

inline void Cpu::AddEip(uint32_t data){
    this->eip += data;
    if(!this->IsProtectedMode()){
        this->eip = this->eip & 0x0000FFFF;
    }
}

inline void Cpu::SetEip(uint32_t addr){
    if(!this->IsProtectedMode()){
        this->eip = this->eip & 0x0000FFFF;
    }
    this->eip = addr;
}

inline bool Cpu::IsProtectedMode() const{
    return (bool)this->cr0.flgs.PE;
}

inline bool Cpu::IsPrefixAddrSize(){
    return this->prefix_flgs[FLG_67];
}

inline bool Cpu::IsPrefixOpSize(){
    return this->prefix_flgs[FLG_66];
}

inline void Cpu::SetR8L(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data){
    *this->gprs[register_type] = ((*this->gprs[register_type])&0xffffff00)|(uint32_t)data;
}

inline void Cpu::SetR8H(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data){
    *this->gprs[register_type] = ((*this->gprs[register_type])&0xffff00FF)|((uint32_t)data<<8);
}

inline void Cpu::SetR8(uint32_t register_type, uint8_t data){
    if(register_type<4){
        this->SetR8L((GENERAL_PURPOSE_REGISTER32)register_type, data);
    }else{
        this->SetR8H((GENERAL_PURPOSE_REGISTER32)(register_type-4), data);
    }
}

inline void Cpu::SetR16(GENERAL_PURPOSE_REGISTER32 register_type, uint16_t data){
    *this->gprs[register_type] = ((*this->gprs[register_type])&0xffff0000)|(uint32_t)data;
}

inline void Cpu::SetR32(GENERAL_PURPOSE_REGISTER32 register_type, uint32_t data){
    *this->gprs[register_type] = data;
}

inline void Cpu::SetDataSelector(SEGMENT_REGISTER register_type){
    this->default_data_selector = register_type;
}

inline void Cpu::SetEflgs(uint32_t eflgs){
    this->eflags.raw = eflgs|0x00000002;;
}

inline uint8_t Cpu::GetR8(uint32_t register_type){
    if(register_type<4){
        return this->GetR8L((GENERAL_PURPOSE_REGISTER32)register_type);
    }else{
        return this->GetR8H((GENERAL_PURPOSE_REGISTER32)(register_type-4));
    }
}

inline uint8_t Cpu::GetR8L(GENERAL_PURPOSE_REGISTER32 register_type){
    return (uint8_t)((*this->gprs[register_type])&0x000000ff);
}

inline uint8_t Cpu::GetR8H(GENERAL_PURPOSE_REGISTER32 register_type){
    return (uint8_t)(((*this->gprs[register_type])>>8)&0xff);
}

inline uint16_t Cpu::GetR16(GENERAL_PURPOSE_REGISTER32 register_type){
    return (*this->gprs[register_type])&0x0000FFFF;
}

inline uint32_t Cpu::GetR32(GENERAL_PURPOSE_REGISTER32 register_type){
    return *this->gprs[register_type];
}

inline uint32_t Cpu::GetEip(){
    return this->eip;
}

inline uint32_t Cpu::GetEflgs(){
    return this->eflags.raw;
}

inline template<typename type>void Cpu::UpdateEflagsForDec(type result, type d1, type d2){
    this->UpdateZF(result);
    this->UpdateSF(result);
    this->UpdatePF(result);
    switch(sizeof(result)){
        case 1:
            this->UpdateOF_Sub8(result, d1, d2);
            break;
        case 2:
            this->UpdateOF_Sub16(result, d1, d2);
            break;
        case 4:
            this->UpdateOF_Sub(result, d1, d2);
            break;
        default:
            this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateElfagsForDec", sizeof(result));
    }
}

inline template<typename type1, typename type2>void Cpu::UpdateEflagsForAdd(type1 result, type2 d1, type2 d2){
    this->UpdateZF((type2)result);
    this->UpdateSF((type2)result);
    this->UpdatePF(result);
    this->UpdateCfForSub(result, sizeof(d1));
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

inline template<typename type>void Cpu::UpdateEflagsForShr(type result){
    this->UpdateZF((uint32_t)result);
    this->UpdateSF(result);
    this->UpdatePF((uint32_t)result);
}

inline template<typename type>void Cpu::UpdateOF_Add(type result, type d1, type d2){
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

inline template<typename type> void Cpu::UpdateCfForSub(type data, int group){
    switch(group){
        case 1:
            this->eflags.flgs.CF = ((data>>8)&1)? 1:0;
            break;
        case 2:
            this->eflags.flgs.CF = ((data>>16)&1)? 1:0;
            break;
        case 4:
        case 8:
            this->eflags.flgs.CF = ((data>>32)&1)? 1:0;
            break;
        default:
            this->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF", sizeof(data));
    }
}

inline template<typename type> void Cpu::UpdateSF(type data){
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

inline template<typename type>void Cpu::UpdateEflagsForAnd(type data){
    this->ClearFlag(CF);
    this->ClearFlag(OF);
    this->UpdateSF(data);
    this->UpdateZF(data);
    this->UpdatePF(data);
}

inline template<typename type>void Cpu::UpdateEflagsForUnsignedMul(type data){
    if(data==0){
        this->ClearFlag(OF);
        this->ClearFlag(CF);
    }else{
        this->SetFlag(OF);
        this->SetFlag(CF);
    }
}

inline bool Cpu::IsException(){
    return this->is_exception_;
}

inline void Cpu::SetException(int error_code){
    this->is_exception_ = true;
    this->error_code_   = error_code;
}

inline void Cpu::SetVectorNumber(int vector_number){
    this->vector_number_ = vector_number;
}

inline int Cpu::GetVectorNumber(){
    return this->vector_number_;
}

inline void Cpu::SetPrefix66(){
    this->prefix_flgs[FLG_66] = true;
}

inline uint8_t Cpu::GetIopl(){
    return this->eflags.flgs.IOPL;
}