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