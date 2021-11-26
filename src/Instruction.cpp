#include "Instruction.h"
#include "Cpu.h"
#include "Memory.h"
#include "IoPort.h"

Instruction::Instruction(string code_name){
    this->code_name = code_name;
}

void Instruction::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    this->Error("Not implemented: Instruction::Run");
}

void Instruction::Push16(Cpu* cpu, Memory* mem, uint16_t data){
    cpu->SetR16(ESP, cpu->GetR16(ESP)-2);
    mem->Write(cpu->GetLinearStackAddr(), data);
}

void Instruction::Push32(Cpu* cpu, Memory* mem, uint32_t data){
    cpu->SetR32(ESP, cpu->GetR32(ESP)-4);
    mem->Write(cpu->GetLinearStackAddr(), data);
}

uint16_t Instruction::Pop16(Cpu* cpu, Memory* mem){
    uint32_t addr;
    uint16_t data;
    addr = cpu->GetLinearStackAddr();
    data = mem->Read16(addr);
    cpu->SetR16(ESP, cpu->GetR16(ESP)+2);
    return data;
}

uint32_t Instruction::Pop32(Cpu* cpu, Memory* mem){
    uint32_t addr;
    uint32_t data;
    addr = cpu->GetLinearStackAddr();
    data = mem->Read32(addr);
    cpu->SetR32(ESP, cpu->GetR32(ESP)+4);
    return data;
}

void Instruction::Show(){
    fprintf(stderr, "%s\n", this->code_name.c_str());
}

void Instruction::SetModRM(ModRM modrm, Sib* sib){
    this->sib = *sib;
    this->modrm = modrm;
}

void Instruction::ParseModRM_32bitsMode(Cpu* cpu, Memory* mem){
    uint8_t code;
    code = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    this->modrm.mod = ((code&0xC0)>>6);
    this->modrm.op_code = ((code&0x38) >> 3);
    this->modrm.rm = code & 0x07;
    cpu->AddEip(1);//ModRMの内容を読み込んだので、次の番地へ
    if((this->modrm.mod==0x01&&this->modrm.rm==0x05) || (this->modrm.mod==0x10&&this->modrm.rm==0x05)){
        cpu->SetDataSelector(SS);
    }
    //SIB判定
    if(this->modrm.mod!=3 && this->modrm.rm==4){
        this->modrm.sib = mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
        //this->sib = new Sib();//メモリ不足になるかも毎回ここで確保
        this->sib.ParseSib(this->modrm.sib, this->modrm.mod);
        if((this->sib.GetBase()==5 && this->modrm.mod==0x00)){
            this->modrm.disp32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
            cpu->AddEip(4);
            this->sib.SetDisp32(this->modrm.disp32);
        }
    }

    //disp取得disp32は
    if((this->modrm.mod==0 && this->modrm.rm==5) || this->modrm.mod==2){
        this->modrm.disp32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
    }else if(this->modrm.mod==1){
        this->modrm.disp8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
    }
}

void Instruction::ParseModRM_16bitsMode(Cpu* cpu, Memory* mem){
    uint8_t code;
    code = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    this->modrm.mod = ((code&0xC0)>>6);
    this->modrm.op_code = ((code&0x38) >> 3);
    this->modrm.rm = code & 0x07;
    cpu->AddEip(1);
    if((this->modrm.mod==0 && this->modrm.rm==6) || this->modrm.mod==2){
        this->modrm.disp16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(2);
    }else if(this->modrm.mod==1){
        this->modrm.disp8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
    }    
}

void Instruction::ParseModRM(Cpu *cpu, Memory* mem){
    if(cpu->Is32bitsMode()){
        if(cpu->IsPrefixAddrSize()){//32bitmodeで16bitアドレスサイズ
            this->ParseModRM_16bitsMode(cpu, mem);
            return;
        }
        this->ParseModRM_32bitsMode(cpu, mem);
        return;
    }else{
        if(cpu->IsPrefixAddrSize()){//16bitmodeで32bitアドレスサイズ
            this->ParseModRM_32bitsMode(cpu, mem);
            return;
        }
        this->ParseModRM_16bitsMode(cpu, mem);
        return;
    }
}

void Instruction::ParseRegIdx(Cpu* cpu, Memory* mem){
    uint8_t code;
    code = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    this->modrm.mod = ((code&0xC0)>>6);
    this->modrm.op_code = ((code&0x38) >> 3);
    this->modrm.rm = code & 0x07;
    cpu->AddEip(1);//ModRMの内容を読み込んだので、次の番地へ
}

uint32_t Instruction::GetEffectiveAddr(Cpu* cpu, Memory* mem){
    uint32_t rm32;
    uint32_t disp8;
    uint32_t disp32;
    uint32_t addr = 0;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                return addr;
            }
            if(this->modrm.rm==0x04){
                return addr;
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return addr;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                disp8 = (int32_t)this->modrm.disp8;
                addr = addr + disp8;
                return addr;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            return addr;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                disp32 = (int32_t)this->modrm.disp32;
                addr = addr+disp32;
                return addr;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            return addr;
        }
        if(this->modrm.mod==3){
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return addr;
        }
    }else{
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                return this->modrm.disp16;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            return addr;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            return addr;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            return addr;
        }
        addr = cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return addr;
    }
}   

uint16_t Instruction::GetR16ForEffectiveAddr(Cpu* cpu){
    uint16_t data=0;
    uint16_t r1;
    uint16_t r2;
    switch (this->modrm.rm){
        case 0:
            r1   = (uint16_t)cpu->GetR32(EBX);
            r2   = (uint16_t)cpu->GetR32(ESI);
            data = r1+r2;
            return data;
        case 1:
            r1   = (uint16_t)cpu->GetR32(EBX);
            r2   = (uint16_t)cpu->GetR32(EDI);
            data = r1+r2;
            return data;
        case 2:
            r1   = (uint16_t)cpu->GetR32(EBP);
            r2   = (uint16_t)cpu->GetR32(ESI);
            data = r1+r2;
            return data;
        case 3:
            r1   = (uint16_t)cpu->GetR32(EBP);
            r2   = (uint16_t)cpu->GetR32(EDI);
            data = r1+r2;
            return data;
        case 4:
            r1   = (uint16_t)cpu->GetR32(ESI);
            data = r1;
            return data;
        case 5:
            r1   = (uint16_t)cpu->GetR32(EDI);
            data = r1;
            return data;
        case 6:
            r1   = (uint16_t)cpu->GetR32(EBP);
            data = r1;
            return data;
        case 7:
            r1   = (uint16_t)cpu->GetR32(EBX);
            data = r1;
            return data;
    }
    return data;
}


uint8_t Instruction::GetRM8(Cpu* cpu, Memory* mem){
    uint8_t rm8;
    uint32_t disp32;
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm8 = mem->Read8(addr);
                return rm8;
            }
            if(this->modrm.rm==4){
                rm8 = mem->Read8(cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8 = mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                rm8 = mem->Read8(cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8 = mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                rm8 = mem->Read8(cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8 = mem->Read8(addr);
            return rm8;
        }
        rm8 = cpu->GetR8(this->modrm.rm);
        return rm8;
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm8   = mem->Read8(this->modrm.disp16);
                return rm8;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8 = mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8  = mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==2){
            disp16 = (int16_t)this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm8  = mem->Read8(addr);
            return rm8;
        }
        rm8 = cpu->GetR8(this->modrm.rm);
        return rm8;
    }
    this->Error("Not implemented at Instruction::GetRM8");
}

uint16_t Instruction::GetRM16(Cpu* cpu, Memory* mem){
    uint16_t rm16;
    uint32_t disp32;
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
            //this->Error("Sib is not implemented at Instruction::GetRM16", this->code_name.c_str());
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm16 = mem->Read16(addr);
                return rm16;
            }
            if(this->modrm.rm==4){
                this->Error("Stopped at Instruction::GetRM16");
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16 = mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==1){
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16 = mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==2){
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16 = mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==3){
            rm16 = cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return rm16;
        }
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm16   = mem->Read16(this->modrm.disp16);
                return rm16;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16 = mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16  = mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm16  = mem->Read16(addr);
            return rm16;
        }
        rm16 = cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return rm16;
    }
}

uint32_t Instruction::GetRM32(Cpu* cpu, Memory* mem){
    uint32_t rm32;
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
            //this->Error("Sib is not implemented at Instruction::GetRM32", this->code_name.c_str());
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm32 = mem->Read32(addr);
                return rm32;
            }
            if(this->modrm.rm==4){
                rm32 = mem->Read32(cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32 = mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                rm32 = mem->Read32(cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32 = mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                rm32 = mem->Read32(cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32 = mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==3){
            rm32 = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return rm32;
        }
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                rm32   = mem->Read32(this->modrm.disp16);
                return rm32;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32 = mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==1){
            disp8 = (int32_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32  = mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            rm32  = mem->Read32(addr);
            return rm32;
        }
        rm32 = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return rm32;
    }
}

void Instruction::SetRM8(Cpu* cpu, Memory* mem, uint8_t data){
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        cpu->SetR8(this->modrm.rm, data);
        return;
    }else{
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                mem->Write(this->modrm.disp16, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        cpu->SetR8(this->modrm.rm, data);
    }
}

void Instruction::SetRM16(Cpu* cpu, Memory* mem, uint16_t data){
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                this->Error("Stopped at Instruction::SetRM16");
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp32 = this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==3){
            cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
            return;
        }
    }else{
        uint16_t addr;
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                /***
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                ***/
                mem->Write(this->modrm.disp16, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr&0x0000FFFF, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr&0x0000FFFF, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr&0x0000FFFF, data);
            return;
        }
        cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
    }
}

void Instruction::SetRM32(Cpu* cpu, Memory* mem, uint32_t data){
    uint32_t addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(cpu);
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                mem->Write(cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==3){
            cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
            return;
        }
    }else{//16bitアドレスサイズ
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = cpu->GetLinearAddrForDataAccess(addr);
                mem->Write(this->modrm.disp16, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(cpu);
            addr = cpu->GetLinearAddrForDataAccess(addr);
            mem->Write(addr, data);
            return;
        }
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
        return;
    }
}

JmpRel8::JmpRel8(string code_name):Instruction(code_name){

}

void JmpRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    int32_t diff;
    cpu->AddEip(1);
    diff = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(diff+1);
}

MovR32Imm32::MovR32Imm32(string code_name):Instruction(code_name){

}

void MovR32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)(mem->Read8(cpu->GetLinearAddrForCodeAccess())-0xB8);
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){    
        cpu->SetR32(register_type, mem->Read32(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(4);
        return;
    }
    cpu->SetR16(register_type, mem->Read16(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(2);
}

MovSregRm16::MovSregRm16(string code_name):Instruction(code_name){

}

void MovSregRm16::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    SEGMENT_REGISTER register_type;
    uint16_t rm16;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
    rm16 = this->GetRM16(cpu, mem);
    cpu->SetR16(register_type, rm16);
}   

MovR8Rm8::MovR8Rm8(string code_name):Instruction(code_name){

}

void MovR8Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t r8;
    uint8_t rm8;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    cpu->SetR8(this->modrm.reg_index, rm8);
    return;
}

CodeC0::CodeC0(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[5] = new ShrRm8Imm8("ShrRm8Imm8");
    this->instructions[7] = new SarRm8Imm8("SarRm8Imm8");
}

void CodeC0::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code C0 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeC6::CodeC6(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new MovRm8Imm8("MovRm8Imm8");
}

void CodeC6::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code C6 %02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code80::Code80(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new AddRm8Imm8("AddRm8Imm8");
    this->instructions[7] = new CmpRm8Imm8("CmpRm8Imm8");
}

void Code80::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 80 %02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code81::Code81(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new AddRm32Imm32("AddRm32Imm32");
    this->instructions[1] = new OrRm32Imm32("OrRm32Imm32");
    this->instructions[4] = new AndRm32Imm32("AndRm32Imm32");
    this->instructions[5] = new SubRm32Imm32("SubRm32Imm32");
    this->instructions[7] = new CmpRm32Imm32("CmpRm32Imm32");
}

void Code81::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 81 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code0F::Code0F(string code_name):Instruction(code_name){
    for(int i=0; i<256; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0x00] = new Code0F00("Code0F00");
    this->instructions[0x01] = new Code0F01("Code0F01"); 
    this->instructions[0x20] = new MovR32CRX("MovR32CRX"); 
    this->instructions[0x22] = new MovCRXR32("MovCRXR32");
    this->instructions[0x84] = new JeRel32("JeRel32"); 
    this->instructions[0x85] = new JneRel32("JneRel32");
    this->instructions[0x86] = new JnaRel32("JnaRel32");
    this->instructions[0x87] = new JaRel32("JaRel32");
    this->instructions[0x88] = new JsRel32("JsRel32");
    this->instructions[0x89] = new JnsRel32("JnsRel32");
    this->instructions[0x8C] = new JlRel32("JlRel32");
    this->instructions[0x8D] = new JgeRel32("JgeRel32");
    this->instructions[0x8E] = new JleRel32("JleRel32");
    this->instructions[0x8F] = new JgRel32("JgRel32");
    this->instructions[0x92] = new SetbRm8("SetbRm8");
    this->instructions[0x94] = new SeteRm8("SeteRm8");
    this->instructions[0x95] = new SetneRm8("SetneRm8");
    this->instructions[0x97] = new SetaRm8("SetaRm8");
    this->instructions[0x9D] = new SetgeRm8("SetgeRm8");
    this->instructions[0x9F] = new SetgRm8("SetgRm8");
    this->instructions[0xAC] = new ShrdRm32R32Imm8("ShrdRm32R32Imm8");
    this->instructions[0xAF] = new ImulR32Rm32("ImulR32Rm32");
    this->instructions[0xB6] = new MovzxR32Rm8("MovzxR32Rm8");
    this->instructions[0xB7] = new MovzxR32Rm16("MovzxR32Rm16");
    this->instructions[0xBE] = new MovsxR32Rm8("MovsxR32Rm8");
    this->instructions[0xBF] = new MovsxR32Rm16("MovsxR32Rm16");
}

void Code0F::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t op_code;
    cpu->AddEip(1);
    op_code = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    if(this->instructions[op_code]==NULL){
        this->Error("code 0F %02X is not implemented %s::ExecuteSelf", op_code, this->code_name.c_str());
    }
    this->instructions[op_code]->Run(cpu, mem, io_port);
    return;
}

CodeC1::CodeC1(string code_name):Instruction(code_name){
    for(int i=0; i<256; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm32Imm8("SalRm32Imm8");
    this->instructions[5] = new ShrRm32Imm8("ShrRm32Imm8");
    this->instructions[7] = new SarRm32Imm8("SarRm32Imm8");
}

void CodeC1::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code C1 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code0F00::Code0F00(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[3] = new LtrRm16("LtrRm16");
}

void Code0F00::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 0F 00 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code0F01::Code0F01(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0x02] = new Lgdt("Lgdt");
    this->instructions[0x03] = new Lidt("Lidt");
}

void Code0F01::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 0F 01 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

Code83::Code83(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new AddRm32Imm8("AddRm32Imm8");
    this->instructions[2] = new AdcRm32Imm8("AdcRm32Imm8");
    this->instructions[1] = new OrRm32Imm8("OrRm32Imm8");
    this->instructions[4] = new AndRm32Imm8("AndRm32Imm8");
    this->instructions[5] = new SubRm32Imm8("SubRm32Imm8");
    this->instructions[6] = new XorRm32Imm8("XorRm32Imm8");
    this->instructions[7] = new CmpRm32Imm8("CmpRm32Imm8");
}

void Code83::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 83 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeF7::CodeF7(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new TestRm32Imm32("TestRm32Imm32");
    this->instructions[2] = new NotRm32("NotRm32");
    this->instructions[3] = new NegRm32("NegRm32");
    this->instructions[4] = new MulRm32("MulRm32");
    this->instructions[6] = new DivRm32("DivRm32");
    this->instructions[7] = new IdivRm32("IdivRm32");
}

void CodeF7::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code F7 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeFE::CodeFE(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[1] = new DecRm8("DecRm8");
}

void CodeFE::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code F7 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeD2::CodeD2(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    //this->instructions[1] = new RorRm8Cl("RorRm8Cl");
}

void CodeD2::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code D2 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeFF::CodeFF(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new IncRm32("IncRm32");
    this->instructions[1] = new DecRm32("DecRm32");
    this->instructions[2] = new CallRm32("CallRm32");
    this->instructions[3] = new CallM1632("CallM1632");
    this->instructions[4] = new JmpRm32("JmpRm32");
    this->instructions[5] = new JmpM1632("JmpM1632");
    this->instructions[6] = new PushRm32("PushRm32");
}

void CodeFF::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code FF /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeD1::CodeD1(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm32("SalRm32");
    this->instructions[5] = new ShrRm32("ShrRm32");
    this->instructions[7] = new SarRm32("SarRm32");
}

void CodeD1::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code D1 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeD3::CodeD3(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm32Cl("SalRm32Cl");
    this->instructions[5] = new ShrRm32Cl("ShrRm32Cl");
    this->instructions[7] = new SarRm32Cl("SarRm32Cl");
}

void CodeD3::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code D3 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}

CodeF6::CodeF6(string code_name):Instruction(code_name){
    for(int i=0; i<INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new TestRm8Imm8("TestRm8Imm8");
}

void CodeF6::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code F6 /%02X is not implemented %s::ExecuteSelf", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(cpu, mem, io_port);
    return;
}
AddRm8Imm8::AddRm8Imm8(string code_name):Instruction(code_name){

}

void AddRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    GENERAL_PURPOSE_REGISTER32 register_type;
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    rm8  = this->GetRM8(cpu, mem);
    this->SetRM8(cpu, mem, rm8+imm8);
    return;
}

AddRm32Imm8::AddRm32Imm8(string code_name):Instruction(code_name){

}


//ADD命令のフラグレジスタ更新処理を今後やる。
void AddRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode()^cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8_32bits;
        uint64_t result;
        rm32 = this->GetRM32(cpu, mem);
        imm8_32bits = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(1);
        result = (uint64_t)rm32+(uint64_t)imm8_32bits;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAdd(result, rm32, imm8_32bits);
        return;
    }
    uint16_t imm8_16bits;
    uint16_t rm16;
    uint32_t result;
    rm16 = this->GetRM16(cpu, mem);
    imm8_16bits = (int16_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    this->SetRM16(cpu, mem, rm16+imm8_16bits);
    cpu->UpdateEflagsForAdd(result, rm16, imm8_16bits);
    cpu->AddEip(1);
    return;
}

AddEaxImm32::AddEaxImm32(string code_name):Instruction(code_name){

}

void AddEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t eax;
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        eax   = cpu->GetR32(EAX);
        cpu->SetR32(EAX, eax+imm32);
        return;
    }
    uint16_t imm16;
    uint16_t ax;
    imm16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(2);
    ax   = cpu->GetR16(EAX);
    cpu->SetR16(EAX, ax+imm16);
    return;
}

AddRm32R32::AddRm32R32(string code_name):Instruction(code_name){

}

void AddRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32, r32;
        uint64_t result;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = rm32+r32;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAdd(result, rm32, r32);
        return;
    }
    uint16_t rm16, r16;
    uint32_t result;
    rm16 = this->GetRM16(cpu, mem);
    r16  = cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16+r16;
    this->SetRM16(cpu, mem, result);
    return;
}

AddR32Rm32::AddR32Rm32(string code_name):Instruction(code_name){

}

void AddR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint64_t result;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = r32 + rm32;
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        cpu->UpdateEflagsForAdd(result, r32, rm32);
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint32_t result;
    rm16 = this->GetRM16(cpu, mem);
    r16  = cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint32_t)r16 + (uint32_t)rm16;
    cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    cpu->UpdateEflagsForAdd(result, r16, rm16);
    return;
}

CmpAlImm8::CmpAlImm8(string code_name):Instruction(code_name){

}

//ADD命令のフラグレジスタ更新処理を今後やる。
void CmpAlImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    uint8_t al;
    uint64_t result;
    cpu->AddEip(1);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    al  = cpu->GetR8L(EAX);
    result = (uint64_t)al - (uint64_t)imm8;
    cpu->UpdateEflagsForSub8(result, al, imm8);
    return;
}

JzRel8::JzRel8(string code_name):Instruction(code_name){

}

//ADD命令のフラグレジスタ更新処理を今後やる。
void JzRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(ZF)){
        cpu->AddEip(rel8);
    }
    return;
}

MovR8Imm8::MovR8Imm8(string code_name):Instruction(code_name){

}


//ADD命令のフラグレジスタ更新処理を今後やる。
void MovR8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    uint32_t register_type;
    register_type = mem->Read8(cpu->GetLinearAddrForCodeAccess())-0xB0;
    cpu->AddEip(1);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    cpu->SetR8(register_type, imm8);
    return;
}

IntImm8::IntImm8(string code_name):Instruction(code_name){

}


//ADD命令のフラグレジスタ更新処理を今後やる。
void IntImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    IdtGate* idt_gate;
    Tss* tss;
    uint16_t selector, cs, ss;
    uint32_t eip, eflags, esp;
    uint32_t offset_addr;
    cpu->AddEip(1);
    selector = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    if(cpu->IsProtectedMode()){
        idt_gate    = cpu->GetIdtGate(selector*8);
        offset_addr = (((uint32_t)idt_gate->offset_high)<<16) | ((uint32_t)idt_gate->offset_low);
        uint8_t idt_gate_dpl = GET_DPL(idt_gate->access_right);
        uint8_t cpl = cpu->GetCpl();
        GdtGate* gdt_gate = cpu->GetGdtGate(idt_gate->selector);
        uint8_t dest_code_segment_dpl = GET_DPL(gdt_gate->access_right);
        if(idt_gate_dpl<cpl){
            this->Error("Not implemented: dpl(idt_gate)<cpl at %s::Run", this->code_name.c_str());
        }
        if(dest_code_segment_dpl<cpl){
            uint16_t ss;
            uint32_t esp;
            Tss* tss;
            ss = cpu->GetR16(SS);
            eflags = cpu->GetEflgs();
            esp = cpu->GetR32(ESP);
            cs = cpu->GetR16(CS);
            eip = cpu->GetEip();
            tss = cpu->GetCurrentTss();
            cpu->SetR16(SS, tss->ss0);
            cpu->SetR32(ESP, tss->esp0);
            this->Push32(cpu, mem, ss);
            this->Push32(cpu, mem, esp);
            this->Push32(cpu, mem, eflags);
            this->Push32(cpu, mem, cs);
            this->Push32(cpu, mem, eip);
            cpu->SetEip(offset_addr);
            cpu->SetR16(CS, idt_gate->selector);
            //cpu->SetRpl(CS, cpl);
            cpu->ClearFlag(IF);
        }else if(dest_code_segment_dpl==cpl){
            this->Push32(cpu, mem, cpu->GetEflgs());
            this->Push32(cpu, mem, cpu->GetR16(CS));
            this->Push32(cpu, mem, cpu->GetEip());
            cpu->SetEip(offset_addr);
            cpu->SetR16(CS, idt_gate->selector);
            //cpu->SetRpl(CS, cpl);
            cpu->ClearFlag(IF);
        }else{//dpl>cpl
            this->Error("Not implemented: dest_code_segment_dpl>cpl at %s::Run", this->code_name.c_str());
        }
        /***
        if(!outer_privilege_level){
            this->Push32(cpu, mem, cpu->GetEflgs());
            this->Push32(cpu, mem, cpu->GetR16(CS));
            this->Push32(cpu, mem, cpu->GetEip());
            cpu->SetEip(offset_addr);
            cpu->SetR16(CS, idt_gate->selector);
            cpu->ClearFlag(IF);//割り込みゲートではClearする
            return;
        }else{
            ss = cpu->GetR16(SS);
            eflags = cpu->GetEflgs();
            esp = cpu->GetR32(ESP);
            cs  = cpu->GetR16(CS);
            eip = cpu->GetEip();
            tss = cpu->GetCurrentTss();
            cpu->SetR16(SS, tss->ss0);
            cpu->SetR32(ESP, tss->esp0);
            this->Push32(cpu, mem, ss);
            this->Push32(cpu, mem, esp);
            this->Push32(cpu, mem, eflags);
            this->Push32(cpu, mem, cs);
            this->Push32(cpu, mem, eip);
            cpu->SetEip(offset_addr);
            cpu->SetR16(CS, idt_gate->selector);
            cpu->ClearFlag(IF);//割り込みゲートではClearする
        }
        ***/
        return;
    }
    cpu->CallFunctionOnRealMode(mem, selector);
    return;
}

Hlt::Hlt(string code_name):Instruction(code_name){

}

void Hlt::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this_thread::sleep_for(std::chrono::milliseconds(10));
}

JaeRel8::JaeRel8(string code_name):Instruction(code_name){

}

void JaeRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);

    if(!cpu->IsFlag(CF)){
        cpu->AddEip(rel8);
    }
    return;
}

MovRm16Sreg::MovRm16Sreg(string code_name):Instruction(code_name){

}

void MovRm16Sreg::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint16_t register_value;
    SEGMENT_REGISTER register_type;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
    register_value = cpu->GetR16(register_type);
    this->SetRM16(cpu, mem, register_value);
    return;
}

CmpRm8Imm8::CmpRm8Imm8(string code_name):Instruction(code_name){

}

void CmpRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    uint8_t rm8;
    uint64_t result;
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    rm8  = this->GetRM8(cpu, mem);
    result = (uint64_t)rm8 - (uint64_t)imm8;
    cpu->UpdateEflagsForSub8(result, rm8, imm8);
    return;
}

JbeRel8::JbeRel8(string code_name):Instruction(code_name){

}

void JbeRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(CF)||cpu->IsFlag(ZF)){
        cpu->AddEip(rel8);
    }
    return;
}

JnzRel8::JnzRel8(string code_name):Instruction(code_name){

}

void JnzRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(!cpu->IsFlag(ZF)){
        cpu->AddEip(rel8);
    }
    return;
}

JcRel8::JcRel8(string code_name):Instruction(code_name){

}

void JcRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(CF)){
        cpu->AddEip(rel8);
    }
    return;
}

MovRm8R8::MovRm8R8(string code_name):Instruction(code_name){

}

void MovRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t r8;
    uint8_t rm8;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    r8  = cpu->GetR8(this->modrm.reg_index); 
    this->SetRM8(cpu, mem, r8);
    return;
}

JmpRel32::JmpRel32(string code_name):Instruction(code_name){

}

void JmpRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->AddEip(1);
        int32_t diff = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(diff+4);
        return;
    }
    cpu->AddEip(1);
    int32_t diff = (int32_t)((int16_t)mem->Read16(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(diff+2);
    return;
}

MovRm8Imm8::MovRm8Imm8(string code_name):Instruction(code_name){

}

void MovRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    GENERAL_PURPOSE_REGISTER32 register_type;
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    this->SetRM8(cpu, mem, imm8);
    return;
}

MovRm32Imm32::MovRm32Imm32(string code_name):Instruction(code_name){

}

void MovRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm32;
        cpu->AddEip(1);
        this->ParseModRM(cpu, mem);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        this->SetRM32(cpu, mem, imm32);
        return;
    }
    uint16_t imm16;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    imm16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(2);
    this->SetRM16(cpu, mem, imm16);
    return;
}

MovMoffs8Al::MovMoffs8Al(string code_name):Instruction(code_name){

}

void MovMoffs8Al::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t al;
    uint32_t addr;
    cpu->AddEip(1);
    al = cpu->GetR8L(EAX);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){
        uint32_t offset32;
        offset32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        addr = cpu->GetLinearAddrForDataAccess(offset32);
        mem->Write(addr, al);
        return;
    }else{
        uint16_t offset16;
        offset16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(2);
        addr     = cpu->GetLinearAddrForDataAccess((uint32_t)offset16);
        mem->Write(addr, al);
    }
    return;
}

OutImm8::OutImm8(string code_name):Instruction(code_name){

}

void OutImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    cpu->AddEip(1);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    return;
}

Nop::Nop(string code_name):Instruction(code_name){

}

void Nop::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    return;
}

Cli::Cli(string code_name):Instruction(code_name){

}

void Cli::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    cpu->ClearFlag(IF);
    return;
}

CallRel32::CallRel32(string code_name):Instruction(code_name){

}

void CallRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        this->Push32(cpu, mem, cpu->GetEip()+4);
        cpu->AddEip(rel32+4);
        return;
    }
    uint16_t rel16;
    rel16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    this->Push16(cpu, mem, cpu->GetIp()+2);
    cpu->AddIp(rel16+2);
    return;
}

InAlImm8::InAlImm8(string code_name):Instruction(code_name){

}

void InAlImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->IsProtectedMode()){
        this->Error("Not implemented: protected mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint8_t imm8;
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    cpu->SetR8L(EAX, io_port->In8(imm8));
    return;
}

AndAlImm8::AndAlImm8(string code_name):Instruction(code_name){

}

void AndAlImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    uint8_t result;
    cpu->AddEip(1);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    result = cpu->GetR8L(EAX)&imm8;
    cpu->SetR8L(EAX, result);
    cpu->UpdateEflagsForAnd(result);
    return;
}

Ret32Near::Ret32Near(string code_name):Instruction(code_name){

}

void Ret32Near::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetEip(this->Pop32(cpu, mem));
        return;
    }
    uint32_t addr;
    addr = 0x0000FFFF&this->Pop16(cpu, mem);
    cpu->SetEip(addr);
    return;
}

Lgdt::Lgdt(string code_name):Instruction(code_name){

}

void Lgdt::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint16_t limit;
    uint32_t base_addr;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t m32;
        m32    = this->GetEffectiveAddr(cpu, mem);
        limit  = mem->Read16(cpu->GetLinearAddrForDataAccess(m32));
        base_addr  = mem->Read32(cpu->GetLinearAddrForDataAccess(m32+2));
        cpu->SetGdtr(limit, base_addr);
        return;
    }
    uint16_t effective_addr;
    effective_addr        = this->GetEffectiveAddr(cpu, mem);
    limit                 = mem->Read16(cpu->GetLinearAddrForDataAccess(effective_addr));
    base_addr             = 0x00FFFFFF&mem->Read32(cpu->GetLinearAddrForDataAccess(effective_addr+2));
    cpu->SetGdtr(limit, base_addr);
    return;
}

Lidt::Lidt(string code_name):Instruction(code_name){

}

void Lidt::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint16_t limit;
    uint32_t m32, base;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        m32    = this->GetEffectiveAddr(cpu, mem);
        limit = mem->Read16(cpu->GetLinearAddrForDataAccess(m32));
        base  = mem->Read32(cpu->GetLinearAddrForDataAccess(m32+2));
        cpu->SetIdtr(limit, base);
    }else{
        this->Error("Not implemented: %s::Run");
    }
    return;
}
MovR32CRX::MovR32CRX(string code_name):Instruction(code_name){

}

void MovR32CRX::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseRegIdx(cpu, mem);
    cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, cpu->GetCr((CONTROL_REGISTER)this->modrm.reg_index));
    return;
}

AndEaxImm32::AndEaxImm32(string code_name):Instruction(code_name){

}

void AndEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t result;
        uint32_t eax;
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        eax = cpu->GetR32(EAX);
        result = eax & imm32;
        cpu->SetR32(EAX, result);
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t imm16;
    uint16_t result;
    uint16_t ax;
    imm16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(2);
    ax = cpu->GetR16(EAX);
    result = ax & imm16;
    cpu->SetR16(EAX, result);
    cpu->UpdateEflagsForAnd(result);
    return;
}

OrRm32Imm8::OrRm32Imm8(string code_name):Instruction(code_name){

}

void OrRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8;
        uint32_t result;
        rm32 = this->GetRM32(cpu, mem);
        imm8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(1);
        result = rm32|imm8;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

MovCRXR32::MovCRXR32(string code_name):Instruction(code_name){

}

void MovCRXR32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseRegIdx(cpu, mem);
    cpu->SetCr((CONTROL_REGISTER)this->modrm.reg_index, cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm));
    return;
}

MovzxR32Rm8::MovzxR32Rm8(string code_name):Instruction(code_name){

}

void MovzxR32Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, (uint32_t)((uint8_t)this->GetRM8(cpu, mem)));
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

MovR32Rm32::MovR32Rm32(string code_name):Instruction(code_name){

}

void MovR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM32(cpu, mem));
        return;
    }
    cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM16(cpu, mem));
    return;
}

MovRm32R32::MovRm32R32(string code_name):Instruction(code_name){

}

void MovRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->SetRM32(cpu, mem, cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
        return;
    }
    this->SetRM16(cpu, mem, cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
    return;
}

SubRm32Imm8::SubRm32Imm8(string code_name):Instruction(code_name){

}

void SubRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm8;
        uint32_t rm32;
        imm8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(1);
        rm32  = this->GetRM32(cpu, mem);
        this->SetRM32(cpu, mem, rm32-imm8);
        result = (uint64_t)rm32 - (uint64_t)imm8;
        cpu->UpdateEflagsForSub(result, rm32, imm8);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

ImulR32Rm32Imm32::ImulR32Rm32Imm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void ImulR32Rm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rm32;
    uint32_t imm32;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        rm32 = this->GetRM32(cpu, mem);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32*imm32);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

SubRm32Imm32::SubRm32Imm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void SubRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint64_t result;
    uint32_t rm32;
    uint32_t imm32;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        rm32 = this->GetRM32(cpu, mem);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        result = (uint64_t)rm32 - (uint64_t)imm32;
        this->SetRM32(cpu, mem, rm32-imm32);
        cpu->UpdateEflagsForSub(result, rm32, imm32);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

ShrRm32Imm8::ShrRm32Imm8(string code_name):Instruction(code_name){

}

void ShrRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint8_t imm8;
        rm32 = this->GetRM32(cpu, mem);
        imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
        if(imm8==1){
            this->Error("imm8==1 is not implemented at %s::Run", this->code_name.c_str());
        }
        for(uint32_t i=0; i<imm8; i++){
            if(rm32&0x01){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            //rm32 = rm32 / 2;
            rm32 = rm32 >> 1;
        }
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
}

JmpPtr1632::JmpPtr1632(string code_name):Instruction(code_name){

}

void JmpPtr1632::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GdtGate* gdt_gate;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t offset;
        uint16_t selector;
        cpu->AddEip(1);
        offset = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        selector = mem->Read16(cpu->GetLinearAddrForCodeAccess());
        gdt_gate = cpu->GetGdtGate(selector);
        if((gdt_gate->access_right&0x1D)==TSS_TYPE){
            cpu->SaveTask(selector);
            cpu->SwitchTask();
            return;
        }
        cpu->SetR16(CS, selector);
        cpu->SetEip(offset);
        return;
    }
    uint32_t offset;
    uint16_t selector;
    cpu->AddEip(1);
    offset = mem->Read32(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(4);
    selector = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->SetR16(CS, selector);
    cpu->SetEip(offset&0x0000FFFF);
    return;
}

PushR32::PushR32(string code_name):Instruction(code_name){

}

void PushR32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)mem->Read8(cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x50);
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->Push32(cpu, mem, cpu->GetR32(register_type));
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

PopR32::PopR32(string code_name):Instruction(code_name){

}

void PopR32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)mem->Read8(cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x58);
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetR32(register_type, this->Pop32(cpu, mem));
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

PushImm8::PushImm8(string code_name):Instruction(code_name){

}

void PushImm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint16_t imm8_16bit;
    uint32_t imm8_32bit;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        imm8_32bit = (int32_t)(int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
        this->Push32(cpu, mem, imm8_32bit);
        cpu->AddEip(1);
        return;
    }
    imm8_16bit = (uint16_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
    this->Push16(cpu, mem, imm8_16bit);
    cpu->AddEip(1);
    return;
}

IncR32::IncR32(string code_name):Instruction(code_name){

}

void IncR32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)mem->Read8(cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x40);
    uint32_t r32;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        r32 = cpu->GetR32(register_type);
        cpu->SetR32(register_type, r32+1);
        cpu->UpdateEflagsForInc(r32+1, r32, 1);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

CmpRm32Imm32::CmpRm32Imm32(string code_name):Instruction(code_name){

}

void CmpRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm32;
        uint32_t rm32;
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        rm32  = this->GetRM32(cpu, mem);
        result = (uint64_t)rm32 - (uint64_t)imm32;
        cpu->UpdateEflagsForSub(result, rm32, imm32);
        return;
    }
    uint32_t result;
    uint16_t imm16;
    uint16_t rm16;
    imm16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(2);
    rm16  = this->GetRM16(cpu, mem);
    result = (uint32_t)rm16 - (uint32_t)imm16;
    cpu->UpdateEflagsForSub16(result, rm16, imm16);
    return;
}

JleRel8::JleRel8(string code_name):Instruction(code_name){

}

void JleRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(ZF) || (cpu->IsFlag(OF)!=cpu->IsFlag(SF))){
        cpu->AddEip(rel8);
    }
    return;
}

AndRm32Imm8::AndRm32Imm8(string code_name):Instruction(code_name){

}

void AndRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm8;
        uint32_t result;
        imm8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(1);
        result = imm8 & this->GetRM32(cpu, mem);
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

XorRm32R32::XorRm32R32(string code_name):Instruction(code_name){

}

void XorRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint32_t result;
        r32    = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32   = this->GetRM32(cpu, mem);
        result = rm32^r32;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint16_t result;
    rm16 = this->GetRM16(cpu, mem);
    r16  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16^r16;
    this->SetRM16(cpu, mem, result);
    cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
    return;
}

RorRm8Cl::RorRm8Cl(string code_name):Instruction(code_name){

}

void RorRm8Cl::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t cl;
    bool flg;
    //cpu->AddEip(1);
    //this->ParseModRM(cpu, mem);
    cl  = cpu->GetR8L(ECX);
    rm8 = this->GetRM8(cpu, mem);
    if(cl==1){
        this->Error("Not implemented: update OF at %s::Run", this->code_name.c_str());
    }
    for(uint8_t i=0; i<cl; i++){
        if(rm8&0x01){
            flg = true;
        }else{
            flg = false;
        }
        rm8 = rm8 >> 1;
        if(flg){
            rm8 = 0x80|rm8;
        }
    }
    if(rm8&0x01){
        cpu->SetFlag(CF);
    }else{
        cpu->ClearFlag(CF);
    }
    this->SetRM8(cpu, mem, rm8);
    return;
}

PushImm32::PushImm32(string code_name):Instruction(code_name){

}

void PushImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->Push32(cpu, mem, mem->Read32(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(4);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

PushFd::PushFd(string code_name):Instruction(code_name){

}

void PushFd::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t eflgs = cpu->GetEflgs();
        this->Push32(cpu, mem, eflgs);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

OutDxAl::OutDxAl(string code_name):Instruction(code_name){

}

void OutDxAl::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    io_port->Out8(cpu->GetR16(EDX), cpu->GetR8L(EAX));
    return;
}

CmpRm32R32::CmpRm32R32(string code_name):Instruction(code_name){

}

void CmpRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint64_t result;
    uint32_t r32;
    uint32_t rm32;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        r32    = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32   = this->GetRM32(cpu, mem);
        result = (uint64_t)rm32 - (uint64_t)r32;
        cpu->UpdateEflagsForSub(result, rm32, r32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

ShrRm8Imm8::ShrRm8Imm8(string code_name):Instruction(code_name){

}

void ShrRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    rm8  = this->GetRM8(cpu, mem);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::ExecuteSelf", this->code_name.c_str());
    }
    for(unsigned int i=0; i<imm8; i++){
        if(rm8&0x01){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm8 = rm8 >> 1;
    }
    this->SetRM8(cpu, mem, rm8);
    cpu->UpdateEflagsForShr(rm8);
}

LeaR32M::LeaR32M(string code_name):Instruction(code_name){

}

void LeaR32M::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t effective_addr = this->GetEffectiveAddr(cpu, mem);
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, effective_addr);
        return;
    }else{
        uint16_t effective_addr = this->GetEffectiveAddr(cpu, mem);
        cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, effective_addr);
    }
}

PopFd::PopFd(string code_name):Instruction(code_name){

}

void PopFd::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t eflgs;
        eflgs = this->Pop32(cpu, mem);
        cpu->SetEflgs(eflgs);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

Leave::Leave(string code_name):Instruction(code_name){

}

void Leave::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetR32(ESP, cpu->GetR32(EBP));
        cpu->SetR32(EBP, this->Pop32(cpu, mem));
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CmpR32Rm32::CmpR32Rm32(string code_name):Instruction(code_name){

}

void CmpR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t rm32;
        uint64_t result;
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(cpu, mem);
        result = (uint64_t)r32 - (uint64_t)rm32;
        cpu->UpdateEflagsForSub(result, r32, rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());

}

JgRel8::JgRel8(string code_name):Instruction(code_name){

}

void JgRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if((!cpu->IsFlag(ZF)) && (cpu->IsFlag(SF)==cpu->IsFlag(OF))){
        cpu->AddEip(rel8);
    }
    return;
}

ImulR32Rm32::ImulR32Rm32(string code_name):Instruction(code_name){

}

void ImulR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32*r32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

MovsxR32Rm16::MovsxR32Rm16(string code_name):Instruction(code_name){

}

void MovsxR32Rm16::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint32_t rm16;
    rm16 = (int32_t)(int16_t)this->GetRM16(cpu, mem);
    cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
    return;
}

PushRm32::PushRm32(string code_name):Instruction(code_name){

}

void PushRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->Push32(cpu, mem, this->GetRM32(cpu, mem));
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

IncRm32::IncRm32(string code_name):Instruction(code_name){

}

void IncRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(cpu, mem);
        this->SetRM32(cpu, mem, rm32+1);
        cpu->UpdateEflagsForInc(rm32+1, rm32, 1);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

DecR32::DecR32(string code_name):Instruction(code_name){

}

void DecR32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)(mem->Read8(cpu->GetLinearAddrForCodeAccess())-0x48);
        cpu->AddEip(1);
        r32 = cpu->GetR32(register_type);
        result = r32 - 1;
        cpu->SetR32(register_type, result);
        cpu->UpdateEflagsForDec(result, r32, (uint32_t)0xFFFFFFFF);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

TestRm8R8::TestRm8R8(string code_name):Instruction(code_name){

}

void TestRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t r8;
    uint8_t rm8;
    uint8_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8(this->modrm.reg_index); 
    result = rm8 & r8;
    cpu->UpdateEflagsForAnd(result);
    return;
}

JnsRel8::JnsRel8(string code_name):Instruction(code_name){

}

void JnsRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(!cpu->IsFlag(SF)){
        cpu->AddEip(rel8);
    }
    return;
}

CmpRm32Imm8::CmpRm32Imm8(string code_name):Instruction(code_name){

}

void CmpRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint64_t result;
    uint32_t imm8;
    uint32_t rm32;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        imm8 = (int32_t)(int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
        rm32  = this->GetRM32(cpu, mem);
        result = (uint64_t)rm32 - (uint64_t)imm8;
        cpu->UpdateEflagsForSub(result, rm32, imm8);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

AndRm32Imm32::AndRm32Imm32(string code_name):Instruction(code_name){

}

void AndRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t result;
        uint32_t rm32;
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        rm32 = this->GetRM32(cpu, mem);
        result = rm32 & imm32;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SalRm32Imm8::SalRm32Imm8(string code_name):Instruction(code_name){

}

void SalRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8;
        rm32 = this->GetRM32(cpu, mem);
        imm8 = (uint32_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
        if(imm8==1){
            this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
        }
        for(uint32_t i=0; i<imm8; i++){
            if(rm32&0x80000000){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            rm32 = rm32 << 1;
        }
        this->SetRM32(cpu, mem, rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

MovsxR32Rm8::MovsxR32Rm8(string code_name):Instruction(code_name){

}

void MovsxR32Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm8;
        rm8 = (int32_t)(int8_t)this->GetRM8(cpu, mem);
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
        return;
    }
    uint16_t rm8;
    rm8 = (int16_t)(int8_t)this->GetRM8(cpu, mem);
    cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
    return;
}

AndR8Rm8::AndR8Rm8(string code_name):Instruction(code_name){

}

void AndR8Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    result = cpu->GetR8(this->modrm.reg_index) & this->GetRM8(cpu, mem);
    cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    cpu->UpdateEflagsForAnd(result);
}

XchgR32Rm32::XchgR32Rm32(string code_name):Instruction(code_name){

}

void XchgR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    //fprintf(stderr, "task_a->flags = %02X\ntask_a->level = %02X\n", mem->Read8(0x00014FF8+4), mem->Read8(0x000015000));
    uint32_t rm32;
    uint32_t r32;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm32 = this->GetRM32(cpu, mem);
    cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32);
}

JneRel32::JneRel32(string code_name):Instruction(code_name){

}

void JneRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel32;
    cpu->AddEip(1);
    rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(4);
    if(!cpu->IsFlag(ZF)){
        cpu->AddEip(rel32);
    }
    return;
}

JeRel32::JeRel32(string code_name):Instruction(code_name){

}

void JeRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel32;
    cpu->AddEip(1);
    rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(4);
    if(cpu->IsFlag(ZF)){
        cpu->AddEip(rel32);
    }
    return;
}

JnaRel32::JnaRel32(string code_name):Instruction(code_name){

}

void JnaRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel32;
    cpu->AddEip(1);
    rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(4);
    if(cpu->IsFlag(ZF)||cpu->IsFlag(CF)){
        cpu->AddEip(rel32);
    }
    return;
}

TestRm32R32::TestRm32R32(string code_name):Instruction(code_name){

}

void TestRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t rm32;
        uint32_t result;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index); 
        result = rm32 & r32;
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JsRel8::JsRel8(string code_name):Instruction(code_name){

}

void JsRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(SF)){
        cpu->AddEip(rel8);
    }
    return;
}

DivRm32::DivRm32(string code_name):Instruction(code_name){

}

void DivRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint64_t r64;
        uint32_t edx, eax;
        edx = cpu->GetR32(EDX);
        eax = cpu->GetR32(EAX);
        rm32 = this->GetRM32(cpu, mem);

        r64  = (((uint64_t)edx)<<((uint64_t)32))| ((uint64_t)eax);
        cpu->SetR32(EAX, r64/((uint64_t)rm32));
        cpu->SetR32(EDX, r64%(uint64_t)rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

NotRm32::NotRm32(string code_name):Instruction(code_name){

}

void NotRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(cpu, mem);
        rm32 = ~rm32;
        this->SetRM32(cpu, mem, rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

JgeRel8::JgeRel8(string code_name):Instruction(code_name){

}

void JgeRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(SF)==cpu->IsFlag(OF)){
        cpu->AddEip(rel8);
    }
    return;
}

SubRm32R32::SubRm32R32(string code_name):Instruction(code_name){

}

void SubRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = (uint64_t)rm32 - (uint64_t)r32;
        this->SetRM32(cpu, mem, rm32-r32);
        cpu->UpdateEflagsForSub(result, rm32, r32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

JleRel32::JleRel32(string code_name):Instruction(code_name){

}

void JleRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        cpu->AddEip(1);
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if(cpu->IsFlag(ZF) || (cpu->IsFlag(OF)!=cpu->IsFlag(SF))){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

SarRm32Imm8::SarRm32Imm8(string code_name):Instruction(code_name){

}

void SarRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8;
        rm32 = this->GetRM32(cpu, mem);
        imm8 = (uint32_t)(mem->Read8(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(1);
        bool flg = (rm32&SIGN_FLG4)? 1:0;
        if(imm8==1){
            this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
        }
        for(uint32_t i=0; i<imm8; i++){
            if(rm32&0x01){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            rm32 = rm32 >> 1;
        }
        rm32 = rm32 | ((flg)?SIGN_FLG4:0);
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);//shrと同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

OrRm32R32::OrRm32R32(string code_name):Instruction(code_name){

}

void OrRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint32_t result;
        cpu->AddEip(1);
        this->ParseModRM(cpu, mem);
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = rm32 | r32;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

Cdq::Cdq(string code_name):Instruction(code_name){

}

void Cdq::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        bool sign_flg = (cpu->GetR32(EAX)&SIGN_FLG4)?true:false;
        if(sign_flg){
            cpu->SetR32(EDX, 0xFFFFFFFF);
        }else{
            cpu->SetR32(EDX, 0x00000000);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

IdivRm32::IdivRm32(string code_name):Instruction(code_name){

}

void IdivRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        int32_t rm32;
        int64_t r64;
        int32_t edx, eax;
        edx = cpu->GetR32(EDX);
        eax = cpu->GetR32(EAX);
        rm32 = this->GetRM32(cpu, mem);
        r64  = (((int64_t)edx)<<((int64_t)32))| ((int64_t)eax);
        cpu->SetR32(EAX, r64/((int64_t)rm32));
        cpu->SetR32(EDX, r64%(int64_t)rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JlRel8::JlRel8(string code_name):Instruction(code_name){

}

void JlRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if(cpu->IsFlag(SF)!=cpu->IsFlag(OF)){
        cpu->AddEip(rel8);
    }
    return;
}

Sti::Sti(string code_name):Instruction(code_name){

}

void Sti::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    cpu->SetFlag(IF);
    return;
}

PushEs::PushEs(string code_name):Instruction(code_name){

}

void PushEs::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t es = 0;
    cpu->AddEip(1);
    es = (uint32_t)cpu->GetR16(ES);
    this->Push32(cpu, mem, es);
    return;
}

PushDs::PushDs(string code_name):Instruction(code_name){

}

void PushDs::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t ds = 0;
    cpu->AddEip(1);
    ds = (uint32_t)cpu->GetR16(DS);
    this->Push32(cpu, mem, ds);
    return;
}

PushAd::PushAd(string code_name):Instruction(code_name){

}

void PushAd::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t esp;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        esp = cpu->GetR32(ESP);
        this->Push32(cpu, mem, cpu->GetR32(EAX));
        this->Push32(cpu, mem, cpu->GetR32(ECX));
        this->Push32(cpu, mem, cpu->GetR32(EDX));
        this->Push32(cpu, mem, cpu->GetR32(EBX));
        this->Push32(cpu, mem, esp);
        this->Push32(cpu, mem, cpu->GetR32(EBP));
        this->Push32(cpu, mem, cpu->GetR32(ESI));
        this->Push32(cpu, mem, cpu->GetR32(EDI));
        return;
    }
    uint16_t sp;
    sp = cpu->GetR16(ESP);
    this->Push16(cpu, mem, cpu->GetR16(EAX));
    this->Push16(cpu, mem, cpu->GetR16(ECX));
    this->Push16(cpu, mem, cpu->GetR16(EDX));
    this->Push16(cpu, mem, cpu->GetR16(EBX));
    this->Push16(cpu, mem, sp);
    this->Push16(cpu, mem, cpu->GetR16(EBP));
    this->Push16(cpu, mem, cpu->GetR16(ESI));
    this->Push16(cpu, mem, cpu->GetR16(EDI));
    return;
}

InAlDx::InAlDx(string code_name):Instruction(code_name){

}

void InAlDx::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t al;
    uint32_t dx;
    cpu->AddEip(1);
    dx = (uint32_t)cpu->GetR16(EDX);
    cpu->SetR8(EAX, io_port->In8(dx));
    return;
}

DecRm32::DecRm32(string code_name):Instruction(code_name){

}

void DecRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t result;
        rm32 = this->GetRM32(cpu, mem);
        result = rm32 - 1;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForDec(result, rm32, (uint32_t)0xFFFFFFFF);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JaRel8::JaRel8(string code_name):Instruction(code_name){

}

void JaRel8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel8;
    cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    if((!cpu->IsFlag(CF))&&(!cpu->IsFlag(ZF))){
        cpu->AddEip(rel8);
    }
    return;
}

PopAd::PopAd(string code_name):Instruction(code_name){

}

void PopAd::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        edi = this->Pop32(cpu, mem);
        esi = this->Pop32(cpu, mem);
        ebp = this->Pop32(cpu, mem);
        esp = this->Pop32(cpu, mem);
        ebx = this->Pop32(cpu, mem);
        edx = this->Pop32(cpu, mem);
        ecx = this->Pop32(cpu, mem);
        eax = this->Pop32(cpu, mem);

        cpu->SetR32(EAX, eax);
        cpu->SetR32(ECX, ecx);
        cpu->SetR32(EDX, edx);
        cpu->SetR32(EBX, ebx);
        //espは無視する
        cpu->SetR32(EBP, ebp);
        cpu->SetR32(ESI, esi);
        cpu->SetR32(EDI, edi);
    }else{
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    }
    return;
}

PopDs::PopDs(string code_name):Instruction(code_name){

}

void PopDs::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t ds = 0;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        ds = this->Pop32(cpu, mem);
        cpu->SetR16(DS, ds);
        return;
    }else{
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    }
    return;
}

PopEs::PopEs(string code_name):Instruction(code_name){

}

void PopEs::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t es = 0;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        es = this->Pop32(cpu, mem);
        cpu->SetR16(ES, es);
        return;
    }else{
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    }
    return;
}

Iretd::Iretd(string code_name):Instruction(code_name){

}

void Iretd::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    bool outer_privilege_level;
    uint16_t cs, ss;
    uint32_t eip, eflags, esp;
    uint8_t cpl, rpl;
    if(!cpu->IsProtectedMode()){
        this->Error("Not implemented: real mode at %s::Run", this->code_name.c_str());
    }
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        eip    = this->Pop32(cpu, mem);
        cs     = this->Pop32(cpu, mem);
        eflags = this->Pop32(cpu, mem);
        rpl    = GET_RPL(cs);//呼び出し元特権レベル
        cpl    = cpu->GetCpl();
        if(rpl>cpl){
            esp = this->Pop32(cpu, mem);
            ss = this->Pop32(cpu, mem);
            cpu->SetR32(ESP, esp);
            cpu->SetR16(SS, ss);
            cpu->SetR16(CS, cs);
            cpu->SetEip(eip);
            cpu->SetEflgs(eflags);
            //cpu->SetCpl(CS, rpl);
        }else if(rpl==cpl){//rpl<=cpl
            cpu->SetR16(CS, cs);
            cpu->SetEip(eip);
            cpu->SetEflgs(eflags);
        }else{
            this->Error("Not implemented: rpl<cpl %s::Run", this->code_name.c_str());
        }
        return;
    }else{
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    }
    return;
}

/***
 *  Tss* tss = cpu->GetTss();//現在のTSS
    bool outer_privilege_level;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        addr     = this->Pop32(cpu, mem);
        cs = this->Pop32(cpu, mem);
        dpl = cs&0x03;
        outer_privilege_level = (dpl>cpu->GetCpl());
        if(outer_privilege_level){
            esp = this->Pop32(cpu, mem);
            ss  = this->Pop32(cpu, mem);
        }
        cpu->SetEip(addr);
        cpu->SetR16(CS, cs);
        if(outer_privilege_level){
            cpu->SetR32(ESP, esp);
            cpu->SetR16(SS, ss);
        }
        return;
    }
***/

MovEaxMoffs32::MovEaxMoffs32(string code_name):Instruction(code_name){

}

void MovEaxMoffs32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->SetR32(EAX, mem->Read32(cpu->GetLinearAddrForDataAccess(mem->Read32(cpu->GetLinearAddrForCodeAccess()))));
        cpu->AddEip(4);
        return;
    }
    
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

MovMoffs32Eax::MovMoffs32Eax(string code_name):Instruction(code_name){

}

void MovMoffs32Eax::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    //op_size = 32bit
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bit_addr
            mem->Write(cpu->GetLinearAddrForDataAccess(mem->Read32(cpu->GetLinearAddrForCodeAccess())), cpu->GetR32(EAX));
            cpu->AddEip(4);
        }else{//16bit_addr
            mem->Write(cpu->GetLinearAddrForDataAccess(mem->Read16(cpu->GetLinearAddrForCodeAccess())), cpu->GetR32(EAX));
            cpu->AddEip(2);
        }
        return;
    }
    //op_size = 16bit
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//32bit_addr
        mem->Write(cpu->GetLinearAddrForDataAccess(mem->Read32(cpu->GetLinearAddrForCodeAccess())), cpu->GetR16(EAX));
        cpu->AddEip(4);
    }else{//16bit_addr
        mem->Write(cpu->GetLinearAddrForDataAccess(mem->Read16(cpu->GetLinearAddrForCodeAccess())), cpu->GetR16(EAX));
        cpu->AddEip(2);
    }
}

SubR32Rm32::SubR32Rm32(string code_name):Instruction(code_name){

}

void SubR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = (uint64_t)r32 - (uint64_t)rm32;
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        cpu->UpdateEflagsForSub(result, r32, rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

DecRm8::DecRm8(string code_name):Instruction(code_name){

}

void DecRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t r8;
    uint8_t result;
    uint8_t d = 0xFF;
    r8 = this->GetRM8(cpu, mem);
    result = r8 - 1;
    this->SetRM8(cpu, mem, result);
    cpu->UpdateEflagsForDec(result, r8, d);
}

OrRm32Imm32::OrRm32Imm32(string code_name):Instruction(code_name){

}

void OrRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t result;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        result = this->GetRM32(cpu, mem)|mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);  
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

NegRm32::NegRm32(string code_name):Instruction(code_name){

}

void NegRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t result;
        uint32_t rm32;
        uint32_t zero = 0;
        rm32 = this->GetRM32(cpu, mem);
        if(rm32==0){
            cpu->ClearFlag(CF);
        }else{
            cpu->SetFlag(CF);
        }
        result = (uint64_t)zero - (uint64_t)rm32;
        this->SetRM32(cpu, mem, zero-rm32);
        cpu->UpdateEflagsForDec(result, zero, rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

TestEaxImm32::TestEaxImm32(string code_name):Instruction(code_name){

}

void TestEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->UpdateEflagsForAnd(cpu->GetR32(EAX)&mem->Read32(cpu->GetLinearAddrForCodeAccess()));
        cpu->AddEip(4);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

JsRel32::JsRel32(string code_name):Instruction(code_name){

}

void JsRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if(cpu->IsFlag(SF)){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

OrEaxImm32::OrEaxImm32(string code_name):Instruction(code_name){

}

void OrEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t result;
        result = cpu->GetR32(EAX) | mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        cpu->SetR32(EAX, result);
        cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

AddRm32Imm32::AddRm32Imm32(string code_name):Instruction(code_name){

}

void AddRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm32;
        uint32_t result;
        rm32  = this->GetRM32(cpu, mem);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        this->SetRM32(cpu, mem, rm32+imm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

XorRm32Imm8::XorRm32Imm8(string code_name):Instruction(code_name){

}

void XorRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm8;
        uint32_t result;
        imm8   =  (int32_t)(int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(1);
        result = this->GetRM32(cpu, mem)^imm8;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SubEaxImm32::SubEaxImm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void SubEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint64_t result;
    uint32_t eax;
    uint32_t imm32;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        eax = cpu->GetR32(EAX);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        result = (uint64_t)eax - (uint64_t)imm32;
        cpu->SetR32(EAX, result);
        cpu->UpdateEflagsForSub(result, eax, imm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JgRel32::JgRel32(string code_name):Instruction(code_name){

}

void JgRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if((!cpu->IsFlag(ZF)) && (cpu->IsFlag(SF)==cpu->IsFlag(OF))){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

ImulR32Rm32Imm8::ImulR32Rm32Imm8(string code_name):Instruction(code_name){

}

void ImulR32Rm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t imm8 = (int32_t)(int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess());
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM32(cpu, mem)*imm8);
        cpu->AddEip(1);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetneRm8::SetneRm8(string code_name):Instruction(code_name){

}

void SetneRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if(!cpu->IsFlag(ZF)){
            this->SetRM8(cpu, mem, 1);
        }else{
            this->SetRM8(cpu, mem, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SubRm8R8::SubRm8R8(string code_name):Instruction(code_name){

}

void SubRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint8_t rm8, r8;
        rm8 = this->GetRM8(cpu, mem);
        r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        uint32_t result = (uint32_t)rm8- (uint32_t)r8;
        this->SetRM8(cpu, mem, result);
        cpu->UpdateEflagsForSub8(result, rm8, r8);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

AdcRm8R8::AdcRm8R8(string code_name):Instruction(code_name){

}

void AdcRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cf  = cpu->IsFlag(CF)?1:0;
    result = (uint16_t)rm8+(uint16_t)r8+(uint16_t)cf;
    this->SetRM8(cpu, mem, result);
    cpu->UpdateEflagsForAdd((uint16_t)result, (uint8_t)rm8, (uint8_t)(r8+cf));
}

Clc::Clc(string code_name):Instruction(code_name){

}

void Clc::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    cpu->ClearFlag(CF);
}

AddR8Rm8::AddR8Rm8(string code_name):Instruction(code_name){

}

void AddR8Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint16_t)rm8+(uint16_t)r8;
    cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    cpu->UpdateEflagsForAdd(result, rm8, r8);
}

SbbRm8R8::SbbRm8R8(string code_name):Instruction(code_name){

}

void SbbRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint8_t rm8, r8;
    uint8_t cf = cpu->IsFlag(CF)?1:0;
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t result = (uint32_t)rm8- ((uint32_t)r8+cf);
    this->SetRM8(cpu, mem, result);
    cpu->UpdateEflagsForSub8(result, rm8, r8+cf);
}

ShrRm32Cl::ShrRm32Cl(string code_name):Instruction(code_name){

}

void ShrRm32Cl::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint8_t cl;
        rm32 = this->GetRM32(cpu, mem);
        cl = cpu->GetR8L(ECX);
        if(cl==1){
            if(rm32&SIGN_FLG4){
                cpu->SetFlag(OF);
            }else{
                cpu->ClearFlag(OF);
            }
        }
        for(uint32_t i=0; i<cl; i++){
            if(rm32&1){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            //rm32 = rm32 / 2;
            rm32 = rm32 >> 1;
        }
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

Xlatb::Xlatb(string code_name):Instruction(code_name){

}

void Xlatb::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//xlatbはaddr_sizeで判別
        uint32_t al;
        al = (uint32_t)cpu->GetR8L(EAX);
        cpu->SetR8L(EAX, mem->Read8(cpu->GetLinearAddrForDataAccess(cpu->GetR32(EBX)))+al);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CmpRm8R8::CmpRm8R8(string code_name):Instruction(code_name){

}

void CmpRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint32_t r8;
    uint32_t rm8;
    uint32_t result;
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(cpu, mem);
    result = (uint32_t)rm8 - (uint32_t)r8;
    cpu->UpdateEflagsForSub8(result, rm8, r8);
}

CmpEaxImm32::CmpEaxImm32(string code_name):Instruction(code_name){

}

void CmpEaxImm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm32;
        uint32_t eax;
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        eax  = cpu->GetR32(EAX);
        result = (uint64_t)eax - (uint64_t)imm32;
        cpu->UpdateEflagsForSub(result, eax, imm32);
        return;
    }
    uint32_t result;
    uint16_t imm16;
    uint16_t ax;
    imm16 = mem->Read16(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(2);
    ax  = cpu->GetR16(EAX);
    result = (uint32_t)ax - (uint32_t)imm16;
    cpu->UpdateEflagsForSub16(result, ax, imm16);
}

TestRm32Imm32::TestRm32Imm32(string code_name):Instruction(code_name){

}

void TestRm32Imm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm32;
        rm32 = this->GetRM32(cpu, mem);
        imm32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        cpu->UpdateEflagsForAnd(rm32&imm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

LtrRm16::LtrRm16(string code_name):Instruction(code_name){

}

void LtrRm16::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->SetTr(this->GetRM16(cpu, mem));
    return;
}

AndRm8R8::AndRm8R8(string code_name):Instruction(code_name){

}

void AndRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t r8;
    uint8_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8 & r8;
    this->SetRM8(cpu, mem, result);
    cpu->UpdateEflagsForAnd(result);
    return;
}

JmpM1632::JmpM1632(string code_name):Instruction(code_name){

}

void JmpM1632::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t addr;
        uint32_t offset_addr;
        uint16_t selector;
        addr = this->GetEffectiveAddr(cpu, mem);
        offset_addr = mem->Read32(cpu->GetLinearAddrForDataAccess(addr));
        selector = mem->Read16(cpu->GetLinearAddrForDataAccess(addr+4));
        GdtGate* gdt_gate = cpu->GetGdtGate(selector);
        if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
            cpu->SetR16(CS, selector);
            cpu->SetEip(offset_addr);
        }else{
            cpu->SaveTask(selector);
            cpu->SwitchTask();
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SeteRm8::SeteRm8(string code_name):Instruction(code_name){

}

void SeteRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if(cpu->IsFlag(ZF)){
            this->SetRM8(cpu, mem, 1);
        }else{
            this->SetRM8(cpu, mem, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

MovAlMoffs8::MovAlMoffs8(string code_name):Instruction(code_name){

}

void MovAlMoffs8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t offset32;
    uint8_t data;
    cpu->AddEip(1);
    offset32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(4);
    data     = mem->Read8(cpu->GetLinearAddrForDataAccess(offset32));
    cpu->SetR8L(EAX, data);
    return;
}

RcrRm8Imm8::RcrRm8Imm8(string code_name):Instruction(code_name){

}

void RcrRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    rm8 = this->GetRM8(cpu, mem);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    for(uint32_t i=0; i<imm8; i++){
        if(rm8&0x01){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm8 = rm8 >> 1;
        rm8 = rm8 | (cpu->IsFlag(CF)?0x80:0x00);
    }
    this->SetRM8(cpu, mem, rm8);
    return;
}

SarRm8Imm8::SarRm8Imm8(string code_name):Instruction(code_name){

}

void SarRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    rm8 = this->GetRM8(cpu, mem);
    imm8 = (uint8_t)(mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    cpu->AddEip(1);
    bool flg = (rm8&SIGN_FLG1)? 1:0;
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    for(uint8_t i=0; i<imm8; i++){
        if(rm8&0x01){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm8 = rm8 >> 1;
    }
    rm8 = rm8 | ((flg)?SIGN_FLG1:0);
    this->SetRM8(cpu, mem, rm8);
    cpu->UpdateEflagsForShr(rm8);
    return;
}

Cld::Cld(string code_name):Instruction(code_name){

}

void Cld::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    cpu->ClearFlag(DF);
    return;
}

CmpsM8M8::CmpsM8M8(string code_name):Instruction(code_name){

}

void CmpsM8M8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t ecx = 1;
    cpu->AddEip(1);
    if(cpu->IsPrefixRep()){
        ecx = cpu->GetR32(ECX);
    }
    if(cpu->IsPrefixRepnz()){
        this->Error("Not implemented: REPNZ at %s::Run", this->code_name.c_str());
    }
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//アドレスで場合分け
        uint32_t base_ds, base_es;
        uint32_t esi, edi;
        uint32_t base_ds_esi, base_es_edi;
        uint8_t m1, m2;
        uint32_t result;
        uint32_t d;
        base_ds = cpu->GetBaseAddr(DS);
        base_es = cpu->GetBaseAddr(ES);
        for(int i=0; i<ecx; i++){
            esi     = cpu->GetR32(ESI);
            edi     = cpu->GetR32(EDI);
            base_ds_esi = base_ds+esi;
            base_es_edi = base_es+edi;
            m1      = mem->Read8(base_ds_esi);
            m2      = mem->Read8(base_es_edi);
            result = (uint32_t)m1 - (uint32_t)m2;
            cpu->UpdateEflagsForSub8(result, m1, m2);
            d = cpu->IsFlag(DF)? 0xFFFFFFFF:0x00000001;
            cpu->SetR32(ESI, esi+d);
            cpu->SetR32(EDI, edi+d);
            if(cpu->IsPrefixRep()){
                cpu->SetR32(ECX, cpu->GetR32(ECX)-1);
                if(!cpu->IsFlag(ZF)){
                    return;
                }
            }
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

SetaRm8::SetaRm8(string code_name):Instruction(code_name){

}

void SetaRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if((!cpu->IsFlag(ZF))&&(!cpu->IsFlag(CF))){
            this->SetRM8(cpu, mem, 1);
        }else{
            this->SetRM8(cpu, mem, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetbRm8::SetbRm8(string code_name):Instruction(code_name){

}

void SetbRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if(cpu->IsFlag(CF)){
            this->SetRM8(cpu, mem, 1);
        }else{
            this->SetRM8(cpu, mem, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

TestRm8Imm8::TestRm8Imm8(string code_name):Instruction(code_name){

}

void TestRm8Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8;
    uint8_t imm8;
    uint8_t result;
    rm8 = this->GetRM8(cpu, mem);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    result = rm8 & imm8;
    cpu->UpdateEflagsForAnd(result);
}

MovzxR32Rm16::MovzxR32Rm16(string code_name):Instruction(code_name){

}

void MovzxR32Rm16::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint32_t rm16 = (uint32_t)this->GetRM16(cpu, mem);
    cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
}

SbbR8Rm8::SbbR8Rm8(string code_name):Instruction(code_name){

}

void SbbR8Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint8_t r8, rm8;
    uint8_t cf = cpu->IsFlag(CF)?1:0;
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(cpu, mem);
    uint32_t result = (uint32_t)r8- ((uint32_t)rm8+cf);
    cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    cpu->UpdateEflagsForSub8(result, r8, rm8+cf);
}

JgeRel32::JgeRel32(string code_name):Instruction(code_name){

}

void JgeRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rel32;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        cpu->AddEip(1);
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if(cpu->IsFlag(SF)==cpu->IsFlag(OF)){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CallPtr1632::CallPtr1632(string code_name):Instruction(code_name){

}

void CallPtr1632::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GdtGate* gdt_gate;
    uint32_t imm32;
    uint16_t selector;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        imm32    = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        selector = mem->Read16(cpu->GetLinearAddrForCodeAccess()+4);
        gdt_gate = cpu->GetGdtGate(selector);
        cpu->AddEip(6);
        if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
            this->Push32(cpu, mem, cpu->GetR16(CS));
            this->Push32(cpu, mem, cpu->GetEip());
            cpu->SetEip(imm32);
            cpu->SetR16(CS, selector);
            return;
        }
        this->Error("Not implemented: TSS at %s::Run", this->code_name.c_str());
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

Ret32Far::Ret32Far(string code_name):Instruction(code_name){

}

void Ret32Far::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t eip;
    uint16_t cs;
    uint32_t esp;
    uint16_t ss;
    uint8_t cpl, rpl;
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        eip     = this->Pop32(cpu, mem);
        cs      = this->Pop32(cpu, mem);
        rpl = GET_RPL(cs);
        cpl = cpu->GetCpl();
        if(rpl>cpl){
            cpu->SetEip(eip);
            cpu->SetR16(CS, cs);
            esp = this->Pop32(cpu, mem);
            ss  = this->Pop32(cpu, mem);
            cpu->SetR32(ESP, esp);
            cpu->SetR16(SS, ss);
        }else{
            cpu->SetEip(eip);
            cpu->SetR16(CS, cs);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CallM1632::CallM1632(string code_name):Instruction(code_name){

}

void CallM1632::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    GdtGate* gdt_gate;
    uint16_t selector;
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t effective_addr;
        uint32_t offset_addr;
        effective_addr = this->GetEffectiveAddr(cpu, mem);
        offset_addr = mem->Read32(cpu->GetLinearAddrForDataAccess(effective_addr));
        selector = mem->Read16(cpu->GetLinearAddrForDataAccess(effective_addr+4));
        gdt_gate = cpu->GetGdtGate(selector);
        if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
            this->Push32(cpu, mem, cpu->GetR16(CS));
            this->Push32(cpu, mem, cpu->GetEip());
            cpu->SetEip(offset_addr);
            cpu->SetR16(CS, selector);
            return;
        }
        this->Error("Not implemented: TSS at %s::Run", this->code_name.c_str());
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

PushSs::PushSs(string code_name):Instruction(code_name){

}

void PushSs::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t es = 0;
    cpu->AddEip(1);
    es = (uint32_t)cpu->GetR16(SS);
    this->Push32(cpu, mem, es);
    return;
}

CmpR8Rm8::CmpR8Rm8(string code_name):Instruction(code_name){

}

void CmpR8Rm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    uint32_t r8;
    uint32_t rm8;
    uint32_t result;
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(cpu, mem);
    result = (uint32_t)r8 - (uint32_t)rm8;
    cpu->UpdateEflagsForSub8(result, r8, rm8);
    return;
}

MulRm32::MulRm32(string code_name):Instruction(code_name){

}

void MulRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t rm32;
        uint64_t eax;
        uint64_t result;
        eax    = cpu->GetR32(EAX);
        rm32   = this->GetRM32(cpu, mem);
        result = eax*rm32;
        cpu->SetR32(EAX, result&0x00000000FFFFFFFF);
        cpu->SetR32(EDX, (result&0xFFFFFFFF00000000)>>32);
        if(cpu->GetR32(EDX)){
            cpu->ClearFlag(OF);
            cpu->ClearFlag(CF);
        }else{
            cpu->SetFlag(OF);
            cpu->SetFlag(CF);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JnsRel32::JnsRel32(string code_name):Instruction(code_name){

}

void JnsRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        cpu->AddEip(1);
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if(!cpu->IsFlag(SF)){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

AddRm8R8::AddRm8R8(string code_name):Instruction(code_name){

}

void AddRm8R8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    rm8 = this->GetRM8(cpu, mem);
    r8  = cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint16_t)rm8+(uint16_t)r8;
    this->SetRM8(cpu, mem, result);
    cpu->UpdateEflagsForAdd(result, r8, rm8);
    return;
}

JaRel32::JaRel32(string code_name):Instruction(code_name){

}

void JaRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        cpu->AddEip(1);
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if((!cpu->IsFlag(CF))&&(!cpu->IsFlag(ZF))){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JlRel32::JlRel32(string code_name):Instruction(code_name){

}

void JlRel32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rel32;
        cpu->AddEip(1);
        rel32 = mem->Read32(cpu->GetLinearAddrForCodeAccess());
        cpu->AddEip(4);
        if(cpu->IsFlag(SF)!=cpu->IsFlag(OF)){
            cpu->AddEip(rel32);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

JmpRm32::JmpRm32(string code_name):Instruction(code_name){

}

void JmpRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32 = this->GetRM32(cpu, mem);
        cpu->SetEip(rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

ShrRm32::ShrRm32(string code_name):Instruction(code_name){

}

void ShrRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t temp_rm32;
        rm32 = this->GetRM32(cpu, mem);
        temp_rm32 = rm32;
        if(rm32&1){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(cpu, mem, rm32);
        if(temp_rm32&0x80000000){
            cpu->SetFlag(OF);
        }else{
            cpu->ClearFlag(OF);
        }
        cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

SalRm32Cl::SalRm32Cl(string code_name):Instruction(code_name){

}

void SalRm32Cl::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t cl;
        bool change_of_flg;
        uint32_t msb_dest;
        rm32 = this->GetRM32(cpu, mem);
        cl = cpu->GetR8L(ECX);
        change_of_flg = cl==1;
        for(uint32_t i=0; i<cl; i++){
            if(rm32&0x80000000){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            rm32 = rm32 << 1;
        }
        this->SetRM32(cpu, mem, rm32);
        if(change_of_flg){
            msb_dest = (0x80000000&rm32)!=0;
            if(msb_dest^cpu->IsFlag(CF)){
                cpu->SetFlag(OF);
            }else{
                cpu->ClearFlag(OF);
            }
        }   
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetgRm8::SetgRm8(string code_name):Instruction(code_name){

}

void SetgRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        if((!cpu->IsFlag(ZF))&&(cpu->IsFlag(CF)==cpu->IsFlag(OF))){
            this->SetRM8(cpu, mem, 1);
        }else{
            this->SetRM8(cpu, mem, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SarRm32Cl::SarRm32Cl(string code_name):Instruction(code_name){

}

void SarRm32Cl::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t cl;
        rm32 = this->GetRM32(cpu, mem);
        cl = cpu->GetR8L(ECX);
        bool flg = (rm32&SIGN_FLG4)? 1:0;
        if(cl==1){
            cpu->ClearFlag(OF);
        }
        for(uint32_t i=0; i<cl; i++){
            if(rm32&0x01){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            rm32 = rm32 >> 1;
        }
        rm32 = rm32 | ((flg)?SIGN_FLG4:0);
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);//shr命令と同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

AndRm32R32::AndRm32R32(string code_name):Instruction(code_name){

}

void AndRm32R32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        uint32_t rm32;
        r32 = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(cpu, mem);
        result = rm32 & r32;
        this->SetRM32(cpu, mem, result);
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SalRm32::SalRm32(string code_name):Instruction(code_name){

}

void SalRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        bool change_of_flg;
        uint32_t msb_dest;
        rm32 = this->GetRM32(cpu, mem);
        if(rm32&0x80000000){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm32 = rm32 << 1;
        this->SetRM32(cpu, mem, rm32);
        msb_dest = (0x80000000&rm32)!=0;
        if(msb_dest^cpu->IsFlag(CF)){
            cpu->SetFlag(OF);
        }else{
            cpu->ClearFlag(OF);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

AndR32Rm32::AndR32Rm32(string code_name):Instruction(code_name){

}

void AndR32Rm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        uint32_t rm32;
        r32 = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(cpu, mem);
        result = rm32 & r32;
        cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        cpu->UpdateEflagsForAnd(result);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

ShrdRm32R32Imm8::ShrdRm32R32Imm8(string code_name):Instruction(code_name){

}

void ShrdRm32R32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint8_t imm8;
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    imm8 = mem->Read8(cpu->GetLinearAddrForCodeAccess());
    cpu->AddEip(1);
    if(imm8==8){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint64_t rm32, r32;
        uint64_t dest;
        rm32 = this->GetRM32(cpu, mem);
        r32  = cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        dest = (r32<<32)|rm32;
        for(int i=0; i<imm8; i++){
            if(dest&0x01){
                cpu->SetFlag(CF);
            }else{
                cpu->ClearFlag(CF);
            }
            dest = dest >> 1;
        }
        rm32 = (uint32_t)dest;
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SarRm32::SarRm32(string code_name):Instruction(code_name){

}

void SarRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(cpu, mem);
        bool flg = (rm32&SIGN_FLG4)? 1:0;
        cpu->ClearFlag(OF);
        if(rm32&0x01){
            cpu->SetFlag(CF);
        }else{
            cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        rm32 = rm32 | ((flg)?SIGN_FLG4:0);
        this->SetRM32(cpu, mem, rm32);
        cpu->UpdateEflagsForShr(rm32);//shr命令と同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetgeRm8::SetgeRm8(string code_name):Instruction(code_name){

}

void SetgeRm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->IsFlag(SF)==cpu->IsFlag(OF)){
        this->SetRM8(cpu, mem, 1);
    }else{
        this->SetRM8(cpu, mem, 0);
    }
    return;
}

Scasb::Scasb(string code_name):Instruction(code_name){

}

void Scasb::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t ecx = 1;
    cpu->AddEip(1);
    if(cpu->IsPrefixRep()){
        this->Error("Not implemented: REP at %s::Run", this->code_name.c_str());
    }
    if(cpu->IsPrefixRepnz()){
        ecx = cpu->GetR32(ECX);
    }
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){//アドレスで場合分け
        uint32_t base_es;
        uint32_t edi;
        uint32_t base_es_edi;
        uint8_t al, m;
        uint32_t result;
        uint32_t d;
        base_es = cpu->GetBaseAddr(ES);
        for(int i=0; i<ecx; i++){
            edi     = cpu->GetR32(EDI);
            base_es_edi = base_es+edi;
            m      = mem->Read8(base_es_edi);
            al = cpu->GetR8L(EAX);
            result = (uint32_t)al - (uint32_t)m;
            cpu->UpdateEflagsForSub8(result, al, m);
            d = cpu->IsFlag(DF)? 0xFFFFFFFF:0x00000001;
            cpu->SetR32(EDI, edi+d);
            if(cpu->IsPrefixRepnz()){
                cpu->SetR32(ECX, cpu->GetR32(ECX)-1);
                if(cpu->IsFlag(ZF)){
                    return;
                }
            }
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

CallRm32::CallRm32(string code_name):Instruction(code_name){

}

void CallRm32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    uint32_t rm32 = this->GetRM32(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
            this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

MovM32M32::MovM32M32(string code_name):Instruction(code_name){

}

void MovM32M32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }else{
        if(cpu->Is32bitsMode() ^ cpu->IsPrefixAddrSize()){
            this->Error("Not implemented: op_size=16bits && addr_size=32bits at %s::Run", this->code_name.c_str());
        }else{
            uint32_t ds, es;
            uint16_t si, di;
            uint16_t d;
            ds = cpu->GetR16(DS)*16;
            si = cpu->GetR16(ESI);
            es = cpu->GetR16(ES)*16;
            di = cpu->GetR16(EDI);
            mem->Write(es+di, mem->Read32(ds+si));
            d = cpu->IsFlag(DF)? -2:1;
            cpu->SetR16(EDI, di+d);
            cpu->SetR16(ESI, si+d);
    
        }
        return;
    }
}

AdcRm32Imm8::AdcRm32Imm8(string code_name):Instruction(code_name){

}

void AdcRm32Imm8::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t imm8;
    uint16_t rm16;
    uint32_t result;
    rm16 = this->GetRM16(cpu, mem);
    imm8 = (int16_t)((int8_t)mem->Read8(cpu->GetLinearAddrForCodeAccess()));
    result = (uint32_t)imm8 + (uint32_t)rm16;
    this->SetRM16(cpu, mem, rm16+imm8);
    cpu->AddEip(1);
    this->Error("Not implemented: %s::Run", this->code_name.c_str());
}
/***
PopM32::PopM32(string code_name):Instruction(code_name){

}

void PopM32::Run(Cpu* cpu, Memory* mem, IoPort* io_port){
    cpu->AddEip(1);
    this->ParseModRM(cpu, mem);
    if(cpu->Is32bitsMode() ^ cpu->IsPrefixOpSize()){
        uint32_t effective_addr;
        effective_addr        = this->GetEffectiveAddr(cpu, mem);
        this->Error("32bits Mode is not implemented at %s::Run", this->code_name.c_str());
        return;
    }
    this->Error("16bits Mode is not implemented at %s::Run", this->code_name.c_str());
    return;
}
***/