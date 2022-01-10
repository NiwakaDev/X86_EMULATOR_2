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