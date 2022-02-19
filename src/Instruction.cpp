#include "Instruction.h"
#include "Instructions.h"
#include "Cpu.h"
#include "Memory.h"
#include "IoPort.h"
#include "Emulator.h"

using namespace std;
using namespace chrono;

#define LSB 0x01
#define MSB_8 0x80
#define MSB_16 0x8000
#define LSB_8 0x01

//Instructionクラスのメンバにアクセスすることがない
//or
//Instructionクラスのpublicメンバにしかアクセスしない関数をInstructionHelper空間に移動させた。
//TODO : InstructionHelper::ShowInstructionNameを外部からもアクセスできるようにする。
namespace InstructionHelper{
    void ShowInstructionName(const Instruction& instruction){
        cout << instruction.GetInstructionName() << endl;
    }
    inline static void Push16(const Emulator& emu, const uint16_t data){
        if(emu.cpu->IsStackAddr32()){
            emu.cpu->SetR32(ESP, emu.cpu->GetR32(ESP)-2);
        }else{
            emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)-2);
        }
        emu.mem->Write(emu.cpu->GetLinearStackAddr(), data);
    }

    inline static void Push32(const Emulator& emu, const uint32_t data){
        if(emu.cpu->IsStackAddr32()){
            emu.cpu->SetR32(ESP, emu.cpu->GetR32(ESP)-4);
        }else{
            emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)-4);
        }
        emu.mem->Write(emu.cpu->GetLinearStackAddr(), data);
    }

    inline static uint8_t Pop8(const Emulator& emu){
        uint32_t addr;
        uint8_t data;
        addr = emu.cpu->GetLinearStackAddr();
        data = emu.mem->Read8(addr);
        emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)+1);
        return data;
    }

    inline static uint16_t Pop16(const Emulator& emu){
        uint32_t addr;
        uint16_t data;
        addr = emu.cpu->GetLinearStackAddr();
        data = emu.mem->Read16(addr);
        if(emu.cpu->IsStackAddr32()){
            emu.cpu->SetR32(ESP, emu.cpu->GetR32(ESP)+2);
        }else{
            emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)+2);
        }
        return data;
    }

    inline static uint32_t Pop32(const Emulator& emu){
        uint32_t addr;
        uint32_t data;
        addr = emu.cpu->GetLinearStackAddr();
        data = emu.mem->Read32(addr);
        if(emu.cpu->IsStackAddr32()){
            emu.cpu->SetR32(ESP, emu.cpu->GetR32(ESP)+4);
        }else{
            emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)+4);
        }
        return data;
    }
}

Instruction::Instruction(string code_name){
    this->code_name = code_name;
}


Instruction::~Instruction(){

}

/***
 * このコードは3代目x86エミュレータに組み込む予定
 * 無理やりこのコードを今のエミュレータに使うのは怖い。
template<typename type>void Instruction::Push(const Emulator& emu, type data){
    //スタックのアドレスサイズはスタックセグメントのBフラグで決定される。
    if(emu.cpu->IsBflg(SS)){//セットされていたら32bitサイズ
        emu.cpu->SetR32(ESP, emu.cpu->GetR32(ESP)-4);
        emu.mem->Write(emu.cpu->GetLinearStackAddr(), data);
    }  
    emu.cpu->SetR16(ESP, emu.cpu->GetR16(ESP)-2);
    emu.mem->Write(emu.cpu->GetLinearStackAddr(), data);
}
***/

inline void Instruction::ParseModRM(const Emulator& emu){
    uint8_t code;
    code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    this->modrm.mod = ((code&0xC0)>>6);
    this->modrm.op_code = ((code&0x38) >> 3);
    this->modrm.rm = code & 0x07;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixAddrSize()){
        if((this->modrm.mod==0x01&&this->modrm.rm==0x05) || (this->modrm.mod==0x10&&this->modrm.rm==0x05)){
            if(!emu.cpu->IsSegmentOverride())emu.cpu->SetDataSelector(SS);
        }
        //SIB判定
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            this->modrm.sib = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(1);
            this->sib.ParseSib(this->modrm.sib, this->modrm.mod);
            if((this->sib.GetBase()==5 && this->modrm.mod==0x00)){
                this->modrm.disp32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
                emu.cpu->AddEip(4);
                this->sib.SetDisp32(this->modrm.disp32);
            }
        }
        //disp取得disp32は
        if((this->modrm.mod==0 && this->modrm.rm==5) || this->modrm.mod==2){
            this->modrm.disp32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(4);
        }else if(this->modrm.mod==1){
            this->modrm.disp8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(1);
        }
    }else{
        if((this->modrm.mod==0 && this->modrm.rm==6) || this->modrm.mod==2){
            this->modrm.disp16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(2);
        }else if(this->modrm.mod==1){
            this->modrm.disp8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(1);
        }   
    }
}

inline void Instruction::ParseRegIdx(const Emulator& emu){
    uint8_t code;
    code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    this->modrm.mod = ((code&0xC0)>>6);
    this->modrm.op_code = ((code&0x38) >> 3);
    this->modrm.rm = code & 0x07;
    emu.cpu->AddEip(1);//ModRMの内容を読み込んだので、次の番地へ
}

inline uint32_t Instruction::GetEffectiveAddr(const Emulator& emu){
    uint32_t disp8;
    uint32_t disp32;
    uint32_t addr = 0;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                return addr;
            }
            if(this->modrm.rm==0x04){
                return addr;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return addr;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                disp8 = (int32_t)this->modrm.disp8;
                addr = addr + disp8;
                return addr;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            return addr;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                disp32 = (int32_t)this->modrm.disp32;
                addr = addr+disp32;
                return addr;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            return addr;
        }
        if(this->modrm.mod==3){
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return addr;
        }
    }else{
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                return this->modrm.disp16;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            return addr;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            return addr;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            return addr;
        }
        addr = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return addr;
    }
}   

inline uint16_t Instruction::GetR16ForEffectiveAddr(const Emulator& emu){
    uint16_t data=0;
    switch (this->modrm.rm){
        uint16_t r1;
        uint16_t r2;
        case 0:
            r1   = (uint16_t)emu.cpu->GetR32(EBX);
            r2   = (uint16_t)emu.cpu->GetR32(ESI);
            data = r1+r2;
            return data;
        case 1:
            r1   = (uint16_t)emu.cpu->GetR32(EBX);
            r2   = (uint16_t)emu.cpu->GetR32(EDI);
            data = r1+r2;
            return data;
        case 2:
            if(!emu.cpu->IsSegmentOverride())emu.cpu->SetDataSelector(SS);
            r1   = (uint16_t)emu.cpu->GetR32(EBP);
            r2   = (uint16_t)emu.cpu->GetR32(ESI);
            data = r1+r2;
            return data;
        case 3:
            if(!emu.cpu->IsSegmentOverride())emu.cpu->SetDataSelector(SS);
            r1   = (uint16_t)emu.cpu->GetR32(EBP);
            r2   = (uint16_t)emu.cpu->GetR32(EDI);
            data = r1+r2;
            return data;
        case 4:
            r1   = (uint16_t)emu.cpu->GetR32(ESI);
            data = r1;
            return data;
        case 5:
            r1   = (uint16_t)emu.cpu->GetR32(EDI);
            data = r1;
            return data;
        case 6:
            if(!emu.cpu->IsSegmentOverride())emu.cpu->SetDataSelector(SS);
            r1   = (uint16_t)emu.cpu->GetR32(EBP);
            data = r1;
            return data;
        case 7:
            r1   = (uint16_t)emu.cpu->GetR32(EBX);
            data = r1;
            return data;
    }
    return data;
}


inline uint8_t Instruction::GetRM8(const Emulator& emu){
    uint8_t rm8;
    uint32_t disp32;
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm8 = emu.mem->Read8(addr);
                return rm8;
            }
            if(this->modrm.rm==4){
                rm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8 = emu.mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                rm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8 = emu.mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                rm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm8;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8 = emu.mem->Read8(addr);
            return rm8;
        }
        rm8 = emu.cpu->GetR8(this->modrm.rm);
        return rm8;
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm8   = emu.mem->Read8(addr);
                return rm8;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8 = emu.mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8  = emu.mem->Read8(addr);
            return rm8;
        }
        if(this->modrm.mod==2){
            disp16 = (int16_t)this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm8  = emu.mem->Read8(addr);
            return rm8;
        }
        rm8 = emu.cpu->GetR8(this->modrm.rm);
        return rm8;
    }
    this->Error("Not implemented at Instruction::GetRM8");
}

inline uint16_t Instruction::GetRM16(const Emulator& emu){
    uint16_t rm16;
    uint32_t disp32;
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
            //this->Error("Sib is not implemented at Instruction::GetRM16", this->code_name.c_str());
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm16 = emu.mem->Read16(addr);
                return rm16;
            }
            if(this->modrm.rm==4){
                rm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm16;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16 = emu.mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                rm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm16;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16 = emu.mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                rm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm16;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16 = emu.mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==3){
            rm16 = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return rm16;
        }
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm16   = emu.mem->Read16(addr);
                return rm16;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16 = emu.mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16  = emu.mem->Read16(addr);
            return rm16;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm16  = emu.mem->Read16(addr);
            return rm16;
        }
        rm16 = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return rm16;
    }
}

inline uint32_t Instruction::GetRM32(const Emulator& emu){
    uint32_t rm32;
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
            //this->Error("Sib is not implemented at Instruction::GetRM32", this->code_name.c_str());
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm32 = emu.mem->Read32(addr);
                return rm32;
            }
            if(this->modrm.rm==4){
                rm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32 = emu.mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                rm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32 = emu.mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                rm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(addr));
                return rm32;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32 = emu.mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==3){
            rm32 = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            return rm32;
        }
    }else{//16bitアドレス
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                rm32   = emu.mem->Read32(addr);
                return rm32;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32 = emu.mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==1){
            disp8 = (int32_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32  = emu.mem->Read32(addr);
            return rm32;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            rm32  = emu.mem->Read32(addr);
            return rm32;
        }
        rm32 = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
        return rm32;
    }
}

inline void Instruction::SetRM8(const Emulator& emu, const uint8_t data){
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                emu.mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        emu.cpu->SetR8(this->modrm.rm, data);
        return;
    }else{
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                emu.mem->Write(addr, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        emu.cpu->SetR8(this->modrm.rm, data);
    }
}

inline void Instruction::SetRM16(const Emulator& emu, const uint16_t data){
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                emu.mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp32 = this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==3){
            emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
            return;
        }
    }else{
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr); 
                emu.mem->Write(addr, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
    }
}

inline void Instruction::SetRM32(const Emulator& emu, const uint32_t data){
    uint32_t addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bitアドレスサイズ
        uint32_t disp8;
        uint32_t disp32;
        if(this->modrm.mod!=3 && this->modrm.rm==4){
            addr = this->sib.GetAddress(*(emu.cpu));
        }
        if(this->modrm.mod==0){
            if(this->modrm.rm==5){
                addr = this->modrm.disp32;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                emu.mem->Write(addr, data);
                return;
            }
            if(this->modrm.rm==4){
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp8;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp8 = (int32_t)this->modrm.disp8;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp8;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            if(this->modrm.rm==4){
                addr = addr + (int32_t)this->modrm.disp32;
                emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(addr), data);
                return;
            }
            disp32 = (int32_t)this->modrm.disp32;
            addr = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm)+disp32;
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==3){
            emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
            return;
        }
    }else{//16bitアドレスサイズ
        uint16_t disp8;
        uint16_t disp16;
        if(this->modrm.mod==0){
            if(this->modrm.rm==6){
                addr = this->modrm.disp16;
                addr = emu.cpu->GetLinearAddrForDataAccess(addr);
                emu.mem->Write(this->modrm.disp16, data);
                return;
            }
            addr = this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==1){
            disp8 = (int16_t)this->modrm.disp8;
            addr  = disp8 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        if(this->modrm.mod==2){
            disp16 = this->modrm.disp16;
            addr  = disp16 + this->GetR16ForEffectiveAddr(emu);
            addr = emu.cpu->GetLinearAddrForDataAccess(addr);
            emu.mem->Write(addr, data);
            return;
        }
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
        return;
    }
}

JmpRel8::JmpRel8(string code_name):Instruction(code_name){

}

void JmpRel8::Run(const Emulator& emu){
    int32_t diff;
    emu.cpu->AddEip(1);
    diff = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(diff+1);
}

MovR32Imm32::MovR32Imm32(string code_name):Instruction(code_name){

}

void MovR32Imm32::Run(const Emulator& emu){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)(emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-0xB8);
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){    
        emu.cpu->SetR32(register_type, emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(4);
        return;
    }
    emu.cpu->SetR16(register_type, emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(2);
}

MovSregRm16::MovSregRm16(string code_name):Instruction(code_name){

}

void MovSregRm16::Run(const Emulator& emu){
    uint32_t eip = emu.cpu->GetEip();
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    SEGMENT_REGISTER register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
    if(register_type==CS){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(-1);
        emu.cpu->SetVectorNumber(CpuEnum::UD);
        return;
    }
    uint16_t rm16                  = this->GetRM16(emu);
    if(emu.cpu->IsProtectedMode()&&((rm16&0xFFFC)/8+1)>((emu.cpu->GetGdtLimit()+1)/8)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    GdtGate* gdt_gate = emu.cpu->GetGdtGate(rm16);
    if(emu.cpu->IsProtectedMode()&&(rm16!=0)&&((gdt_gate->access_right&CpuEnum::S)==0)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    if(emu.cpu->IsProtectedMode()&&(rm16!=0)&&((gdt_gate->access_right&CpuEnum::P)==0)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        if(register_type!=SS){
            emu.cpu->SetVectorNumber(CpuEnum::NP);
        }else{
            emu.cpu->SetVectorNumber(CpuEnum::SS_VECTOR);
        }
        return;
    }
    if(emu.cpu->IsProtectedMode()&&rm16==0&&register_type==SS){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(0);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    uint8_t dpl = CpuHelper::GetDpl(gdt_gate->access_right);
    uint8_t rpl = CpuHelper::GetRpl(rm16);
    uint8_t cpl = emu.cpu->GetCpl();
    if(emu.cpu->IsProtectedMode()&&cpl>dpl&&rpl>dpl){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    if(emu.cpu->IsProtectedMode()&&(register_type==SS)&&(cpl!=rpl)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    if(emu.cpu->IsProtectedMode()&&(register_type==SS)&&((gdt_gate->access_right&0x000F)==CpuEnum::READ_AND_ACCESS)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    if(emu.cpu->IsProtectedMode()&&(register_type==SS)&&((gdt_gate->access_right&0x000F)==CpuEnum::READ_ONLY)){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(rm16&0xFFFC);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    emu.cpu->SetR16(register_type, rm16&0xFFFC);
}   

MovR8Rm8::MovR8Rm8(string code_name):Instruction(code_name){

}

void MovR8Rm8::Run(const Emulator& emu){
    uint8_t rm8;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    emu.cpu->SetR8(this->modrm.reg_index, rm8);
    return;
}

CodeC0::CodeC0(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm8Imm8("SalRm8Imm8");
    this->instructions[5] = new ShrRm8Imm8("ShrRm8Imm8");
    this->instructions[7] = new SarRm8Imm8("SarRm8Imm8");
}

CodeC0::~CodeC0(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeC0::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code C0 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeC6::~CodeC6(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

CodeC6::CodeC6(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new MovRm8Imm8("MovRm8Imm8");
}

void CodeC6::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: C6 %02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

Code80::Code80(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new AddRm8Imm8("AddRm8Imm8");
    this->instructions[1] = new OrRm8Imm8("OrRm8Imm8");
    this->instructions[2] = new AdcRm8Imm8("AdcRm8Imm8");
    this->instructions[4] = new AndRm8Imm8("AndRm8Imm8");
    this->instructions[5] = new SubRm8Imm8("SubRm8Imm8");
    this->instructions[6] = new XorRm8Imm8("XorRm8Imm8");
    this->instructions[7] = new CmpRm8Imm8("CmpRm8Imm8");
}

Code80::~Code80(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void Code80::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: 80 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

Code81::Code81(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new AddRm32Imm32("AddRm32Imm32");
    this->instructions[1] = new OrRm32Imm32("OrRm32Imm32");
    this->instructions[4] = new AndRm32Imm32("AndRm32Imm32");
    this->instructions[5] = new SubRm32Imm32("SubRm32Imm32");
    this->instructions[6] = new XorRm32Imm32("XorRm32Imm32");
    this->instructions[7] = new CmpRm32Imm32("CmpRm32Imm32");
}

Code81::~Code81(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void Code81::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: 81 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
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
    this->instructions[0x80] = new JccRel32("JccRel32");
    this->instructions[0x81] = new JccRel32("JccRel32");
    this->instructions[0x82] = new JccRel32("JccRel32");
    this->instructions[0x83] = new JccRel32("JccRel32");
    this->instructions[0x84] = new JccRel32("JccRel32"); 
    this->instructions[0x85] = new JccRel32("JccRel32");
    this->instructions[0x86] = new JccRel32("JccRel32");
    this->instructions[0x87] = new JccRel32("JccRel32");
    this->instructions[0x88] = new JccRel32("JccRel32");
    this->instructions[0x89] = new JccRel32("JccRel32");
    this->instructions[0x8A] = new JccRel32("JccRel32");
    this->instructions[0x8B] = new JccRel32("JccRel32");
    this->instructions[0x8C] = new JccRel32("JccRel32");
    this->instructions[0x8D] = new JccRel32("JccRel32");
    this->instructions[0x8E] = new JccRel32("JccRel32");
    this->instructions[0x8F] = new JccRel32("JccRel32");
    this->instructions[0x92] = new SetbRm8("SetbRm8");
    this->instructions[0x94] = new SeteRm8("SeteRm8");
    this->instructions[0x95] = new SetneRm8("SetneRm8");
    this->instructions[0x97] = new SetaRm8("SetaRm8");
    this->instructions[0x9D] = new SetgeRm8("SetgeRm8");
    this->instructions[0x9F] = new SetgRm8("SetgRm8");
    this->instructions[0xA0] = new PushFs("PushFs");
    this->instructions[0xA1] = new PopFs("PopFs");
    this->instructions[0xA8] = new PushGs("PushGs");
    this->instructions[0xA9] = new PopGs("PopGs");
    this->instructions[0xAC] = new ShrdRm32R32Imm8("ShrdRm32R32Imm8");
    this->instructions[0xAF] = new ImulR32Rm32("ImulR32Rm32");
    this->instructions[0xB2] = new LssR32M1632("LssR32M1632");
    this->instructions[0xB4] = new LfsR32M1632("LfsR32M1632");
    this->instructions[0xB5] = new LgsR32M1632("LgsR32M1632");
    this->instructions[0xB6] = new MovzxR32Rm8("MovzxR32Rm8");
    this->instructions[0xB7] = new MovzxR32Rm16("MovzxR32Rm16");
    this->instructions[0xBE] = new MovsxR32Rm8("MovsxR32Rm8");
    this->instructions[0xBF] = new MovsxR32Rm16("MovsxR32Rm16");
}

Code0F::~Code0F(){
    for(int i=0; i<256; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void Code0F::Run(const Emulator& emu){
    uint8_t op_code;
    emu.cpu->AddEip(1);
    op_code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    if(this->instructions[op_code]==NULL){
        this->Error("Not implemented: 0F %02X at %s::Run", op_code, this->code_name.c_str());
    }
    this->instructions[op_code]->Run(emu);
    return;
}

CodeC1::CodeC1(string code_name):Instruction(code_name){
    for(int i=0; i<256; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new RolRm32Imm8("RolRm32Imm8");
    this->instructions[2] = new RclRm32Imm8("RclRm32Imm8");
    this->instructions[4] = new SalRm32Imm8("SalRm32Imm8");
    this->instructions[5] = new ShrRm32Imm8("ShrRm32Imm8");
    this->instructions[7] = new SarRm32Imm8("SarRm32Imm8");
}

CodeC1::~CodeC1(){
    for(int i=0; i<256; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeC1::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: C1 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

Code0F00::Code0F00(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[2] = new LldtRm16("LldtRm16");
    this->instructions[3] = new LtrRm16("LtrRm16");
}

Code0F00::~Code0F00(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}


void Code0F00::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: 0F 00 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

Code0F01::Code0F01(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0x02] = new Lgdt("Lgdt");
    this->instructions[0x03] = new Lidt("Lidt");
}

Code0F01::~Code0F01(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}


void Code0F01::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: 0F 01 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

Code83::Code83(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[1] = new OrRm32Imm8("OrRm32Imm8");
    this->instructions[0] = new AddRm32Imm8("AddRm32Imm8");
    this->instructions[2] = new AdcRm32Imm8("AdcRm32Imm8");
    this->instructions[3] = new SbbRm32Imm8("SbbRm32Imm8");
    this->instructions[4] = new AndRm32Imm8("AndRm32Imm8");
    this->instructions[5] = new SubRm32Imm8("SubRm32Imm8");
    this->instructions[6] = new XorRm32Imm8("XorRm32Imm8");
    this->instructions[7] = new CmpRm32Imm8("CmpRm32Imm8");
}

Code83::~Code83(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void Code83::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code 83 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeF7::CodeF7(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new TestRm32Imm32("TestRm32Imm32");
    this->instructions[2] = new NotRm32("NotRm32");
    this->instructions[3] = new NegRm32("NegRm32");
    this->instructions[4] = new MulRm32("MulRm32");
    this->instructions[5] = new ImulRm16("ImulRm16");
    this->instructions[6] = new DivRm32("DivRm32");
    this->instructions[7] = new IdivRm32("IdivRm32");
}

void CodeF7::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: F7 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeFE::CodeFE(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new IncRm8("IncRm8");
    this->instructions[1] = new DecRm8("DecRm8");
}

CodeFE::~CodeFE(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}


void CodeFE::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: FE /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeD0::CodeD0(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[1] = new RorRm8("RorRm8");
    this->instructions[4] = new SalRm8("SalRm8");
    this->instructions[5] = new ShrRm8("ShrRm8");
}

CodeD0::~CodeD0(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeD0::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: D0 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeD2::CodeD2(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm8Cl("SalRm8Cl");
}

CodeD2::~CodeD2(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeD2::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: D2 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeFF::CodeFF(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
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

CodeFF::~CodeFF(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}


void CodeFF::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: FF /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeD1::CodeD1(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[2] = new RclRm32("RclRm32");
    this->instructions[3] = new RcrRm32("RcrRm32");
    this->instructions[4] = new SalRm32("SalRm32");
    this->instructions[5] = new ShrRm32("ShrRm32");
    this->instructions[7] = new SarRm32("SarRm32");
}

CodeD1::~CodeD1(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeD1::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("code D1 /%02X is not implemented %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeD3::CodeD3(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[4] = new SalRm32Cl("SalRm32Cl");
    this->instructions[5] = new ShrRm32Cl("ShrRm32Cl");
    this->instructions[7] = new SarRm32Cl("SarRm32Cl");
}

CodeD3::~CodeD3(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeD3::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: D3 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

CodeF6::CodeF6(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0] = new TestRm8Imm8("TestRm8Imm8");
    this->instructions[2] = new NotRm8("NotRm8");
    this->instructions[4] = new MulRm8("MulRm8");
    this->instructions[6] = new DivRm8("DivRm8");
}

CodeF6::~CodeF6(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeF6::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(this->instructions[this->modrm.reg_index]==NULL){
            this->Error("Not implemented: F6 /%02X at %s::Run", this->modrm.reg_index, this->code_name.c_str());
    }
    this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
    this->instructions[this->modrm.reg_index]->Run(emu);
    return;
}

AddRm8Imm8::AddRm8Imm8(string code_name):Instruction(code_name){

}

void AddRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    uint16_t result;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    rm8  = this->GetRM8(emu);
    result = rm8+imm8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm8, imm8);
    return;
}

AddRm32Imm8::AddRm32Imm8(string code_name):Instruction(code_name){

}


//ADD命令のフラグレジスタ更新処理を今後やる。
void AddRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8_32bits;
        uint64_t result;
        rm32 = this->GetRM32(emu);
        imm8_32bits = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        result = (uint64_t)rm32+(uint64_t)imm8_32bits;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAdd(result, rm32, imm8_32bits);
        return;
    }
    uint16_t imm8;
    uint16_t rm16;
    uint32_t result;
    rm16 = this->GetRM16(emu);
    imm8 = (int16_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    result = (uint32_t)rm16+(uint32_t)imm8;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm16, imm8);
    emu.cpu->AddEip(1);
    return;
}

AddEaxImm32::AddEaxImm32(string code_name):Instruction(code_name){

}

void AddEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t eax;
        uint64_t result;
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        eax   = emu.cpu->GetR32(EAX);
        result = eax+imm32;
        emu.cpu->SetR32(EAX, result);
        emu.cpu->UpdateEflagsForAdd(result, eax, imm32);
        return;
    }
    uint16_t imm16;
    uint16_t ax;
    uint32_t result;
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    ax   = emu.cpu->GetR16(EAX);
    result = (uint32_t)ax+(uint32_t)imm16;
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForAdd(result, ax, imm16);
    return;
}

AddRm32R32::AddRm32R32(string code_name):Instruction(code_name){

}

void AddRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32, r32;
        uint64_t result;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = rm32+r32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAdd(result, rm32, r32);
        return;
    }
    uint16_t rm16, r16;
    uint32_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint32_t)rm16+(uint32_t)r16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm16, r16);
    return;
}

AddR32Rm32::AddR32Rm32(string code_name):Instruction(code_name){

}

void AddR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint64_t result;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = r32 + rm32;
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        emu.cpu->UpdateEflagsForAdd(result, r32, rm32);
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint32_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint32_t)r16 + (uint32_t)rm16;
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAdd(result, r16, rm16);
    return;
}

CmpAlImm8::CmpAlImm8(string code_name):Instruction(code_name){

}

//ADD命令のフラグレジスタ更新処理を今後やる。
void CmpAlImm8::Run(const Emulator& emu){
    uint8_t imm8;
    uint8_t al;
    uint32_t result;
    emu.cpu->AddEip(1);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    al  = emu.cpu->GetR8L(EAX);
    result = (uint32_t)al - (uint32_t)imm8;
    emu.cpu->UpdateEflagsForSub8(result, al, imm8);
    return;
}

JzRel8::JzRel8(string code_name):Instruction(code_name){

}

//ADD命令のフラグレジスタ更新処理を今後やる。
void JzRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(ZF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

MovR8Imm8::MovR8Imm8(string code_name):Instruction(code_name){

}


//ADD命令のフラグレジスタ更新処理を今後やる。
void MovR8Imm8::Run(const Emulator& emu){
    uint8_t imm8;
    uint32_t register_type;
    register_type = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-0xB0;
    emu.cpu->AddEip(1);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    emu.cpu->SetR8(register_type, imm8);
    return;
}

IntImm8::IntImm8(string code_name):Instruction(code_name){

}

void IntImm8::Run(const Emulator& emu){
    IdtGate* idt_gate;
    uint16_t selector, cs, ss;
    uint32_t eip, eflags, esp;
    uint32_t offset_addr;
    emu.cpu->AddEip(1);
    selector = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    if(emu.cpu->IsProtectedMode()){
        idt_gate    = emu.cpu->GetIdtGate(selector*8);
        offset_addr = (((uint32_t)idt_gate->offset_high)<<16) | ((uint32_t)idt_gate->offset_low);
        uint8_t idt_gate_dpl = CpuHelper::GetDpl(idt_gate->access_right);
        uint8_t cpl = emu.cpu->GetCpl();
        GdtGate* gdt_gate = emu.cpu->GetGdtGate(idt_gate->selector);
        uint8_t dest_code_segment_dpl = CpuHelper::GetDpl(gdt_gate->access_right);
        if(idt_gate_dpl<cpl){
            this->Error("Not implemented: dpl(idt_gate)<cpl at %s::Run", this->code_name.c_str());
        }
        if(dest_code_segment_dpl<cpl){
            uint16_t ss;
            uint32_t esp;
            Tss* tss;
            ss = emu.cpu->GetR16(SS);
            eflags = emu.cpu->GetEflgs();
            esp = emu.cpu->GetR32(ESP);
            cs = emu.cpu->GetR16(CS);
            eip = emu.cpu->GetEip();
            tss = emu.cpu->GetCurrentTss();
            emu.cpu->SetR16(SS, tss->ss0);
            emu.cpu->SetR32(ESP, tss->esp0);
            InstructionHelper::Push32(emu, ss);
            InstructionHelper::Push32(emu, esp);
            InstructionHelper::Push32(emu, eflags);
            InstructionHelper::Push32(emu, cs);
            InstructionHelper::Push32(emu, eip);
            emu.cpu->SetEip(offset_addr);
            emu.cpu->SetR16(CS, idt_gate->selector);
            //emu.cpu->SetRpl(CS, cpl);
            emu.cpu->ClearFlag(IF);
        }else if(dest_code_segment_dpl==cpl){
            InstructionHelper::Push32(emu, emu.cpu->GetEflgs());
            InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
            InstructionHelper::Push32(emu, emu.cpu->GetEip());
            emu.cpu->SetEip(offset_addr);
            emu.cpu->SetR16(CS, idt_gate->selector);
            //emu.cpu->SetRpl(CS, cpl);
            emu.cpu->ClearFlag(IF);
        }else{//dpl>cpl
            this->Error("Not implemented: dest_code_segment_dpl>cpl at %s::Run", this->code_name.c_str());
        }
        return;
    }
    if(selector<0x20){
        emu.cpu->CallFunctionOnRealMode(selector);
    }else{
        //PUSH EFLAGSの下位16bit
        //IFをクリア
        //TFをクリア
        //ACをクリア
        //PUSH CS
        //PUSH IP
        //割り込みテーブルからCSとIPを取り出す
        //割り込みテーブルでは、IP、CSの順に並んでいる。
        uint16_t eflags = emu.cpu->GetEflgs();
        uint16_t ip     = emu.cpu->GetEip();
        uint16_t new_ip, new_cs;
        InstructionHelper::Push16(emu, eflags);
        emu.cpu->ClearFlag(IF);
        emu.cpu->ClearFlag(TF);
        emu.cpu->ClearFlag(AC);
        InstructionHelper::Push16(emu, emu.cpu->GetR16(CS));
        InstructionHelper::Push16(emu, ip);
        new_ip = emu.mem->Read16((selector<<2));
        new_cs = emu.mem->Read16((selector<<2)+2);
        emu.cpu->SetR16(CS, new_cs);
        emu.cpu->SetEip(new_ip);
    }
    return;
}

Hlt::Hlt(string code_name):Instruction(code_name){

}

void Hlt::Run(const Emulator& emu){
    if(emu.cpu->IsProtectedMode()&&emu.cpu->GetCpl()!=0){
        emu.cpu->SetException(0);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
        return;
    }
    emu.cpu->AddEip(1);
    this_thread::sleep_for(milliseconds(10));
}

JaeRel8::JaeRel8(string code_name):Instruction(code_name){

}

void JaeRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);

    if(!emu.cpu->IsFlag(CF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

MovRm16Sreg::MovRm16Sreg(string code_name):Instruction(code_name){

}

void MovRm16Sreg::Run(const Emulator& emu){
    uint16_t register_value;
    SEGMENT_REGISTER register_type;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
    register_value = emu.cpu->GetR16(register_type);
    this->SetRM16(emu, register_value);
    return;
}

CmpRm8Imm8::CmpRm8Imm8(string code_name):Instruction(code_name){

}

void CmpRm8Imm8::Run(const Emulator& emu){
    uint8_t imm8;
    uint8_t rm8;
    uint32_t result;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    rm8  = this->GetRM8(emu);
    result = (uint32_t)rm8 - (uint32_t)imm8;
    emu.cpu->UpdateEflagsForSub8(result, rm8, imm8);
    return;
}

JbeRel8::JbeRel8(string code_name):Instruction(code_name){

}

void JbeRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(CF)||emu.cpu->IsFlag(ZF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JnzRel8::JnzRel8(string code_name):Instruction(code_name){

}

void JnzRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsFlag(ZF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JcRel8::JcRel8(string code_name):Instruction(code_name){

}

void JcRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(CF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

MovRm8R8::MovRm8R8(string code_name):Instruction(code_name){

}

void MovRm8R8::Run(const Emulator& emu){
    uint8_t r8;
    uint8_t rm8;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    r8  = emu.cpu->GetR8(this->modrm.reg_index); 
    this->SetRM8(emu, r8);
    return;
}

JmpRel32::JmpRel32(string code_name):Instruction(code_name){

}

void JmpRel32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->AddEip(1);
        int32_t diff = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(diff+4);
        return;
    }
    emu.cpu->AddEip(1);
    int32_t diff = (int32_t)((int16_t)emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(diff+2);
    return;
}

MovRm8Imm8::MovRm8Imm8(string code_name):Instruction(code_name){

}

void MovRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    GENERAL_PURPOSE_REGISTER32 register_type;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    this->SetRM8(emu, imm8);
    return;
}

MovRm32Imm32::MovRm32Imm32(string code_name):Instruction(code_name){

}

void MovRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm32;
        emu.cpu->AddEip(1);
        this->ParseModRM(emu);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        this->SetRM32(emu, imm32);
        return;
    }
    uint16_t imm16;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    this->SetRM16(emu, imm16);
    return;
}

MovMoffs8Al::MovMoffs8Al(string code_name):Instruction(code_name){

}

void MovMoffs8Al::Run(const Emulator& emu){
    uint8_t al;
    uint32_t addr;
    emu.cpu->AddEip(1);
    al = emu.cpu->GetR8L(EAX);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t offset32;
        offset32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        addr = emu.cpu->GetLinearAddrForDataAccess(offset32);
        emu.mem->Write(addr, al);
        return;
    }else{
        uint16_t offset16;
        offset16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(2);
        addr     = emu.cpu->GetLinearAddrForDataAccess((uint32_t)offset16);
        emu.mem->Write(addr, al);
    }
    return;
}

OutImm8::OutImm8(string code_name):Instruction(code_name){

}

void OutImm8::Run(const Emulator& emu){
    uint8_t imm8;
    emu.cpu->AddEip(1);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    return;
}

Nop::Nop(string code_name):Instruction(code_name){

}

void Nop::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    return;
}

/***
void MovSregRm16::Run(const Emulator& emu){
    uint32_t eip = emu.cpu->GetEip();
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    SEGMENT_REGISTER register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
    if(register_type==CS){
        emu.cpu->SetEip(eip);
        emu.cpu->SetException();
        emu.cpu->SetVectorNumber(CpuEnum::UD);
        return;
    }
    uint16_t rm16                  = this->GetRM16(emu);
    emu.cpu->SetR16(register_type, rm16);
}   
***/

Cli::Cli(string code_name):Instruction(code_name){

}

void Cli::Run(const Emulator& emu){
    uint32_t eip = emu.cpu->GetEip();
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsProtectedMode()){
        emu.cpu->ClearFlag(IF);
        return;
    }
    if(emu.cpu->GetIopl()>=emu.cpu->GetCpl()){
        emu.cpu->ClearFlag(IF);
    }else{
        emu.cpu->SetEip(eip);
        emu.cpu->SetException(0);
        emu.cpu->SetVectorNumber(CpuEnum::GP);
    }
    return;
}

CallRel32::CallRel32(string code_name):Instruction(code_name){

}

void CallRel32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rel32;
        rel32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        InstructionHelper::Push32(emu, emu.cpu->GetEip()+4);
        emu.cpu->AddEip(rel32+4);
        return;
    }
    uint16_t rel16;
    rel16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    InstructionHelper::Push16(emu, emu.cpu->GetEip()+2);
    emu.cpu->AddEip(rel16+2);
    return;
}

InAlImm8::InAlImm8(string code_name):Instruction(code_name){

}

void InAlImm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsProtectedMode()){
        this->Error("Not implemented: protected mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint8_t imm8;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    emu.cpu->SetR8L(EAX, emu.io_port->In8(imm8));
    return;
}

AndAlImm8::AndAlImm8(string code_name):Instruction(code_name){

}

void AndAlImm8::Run(const Emulator& emu){
    uint8_t imm8;
    uint8_t result;
    emu.cpu->AddEip(1);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = emu.cpu->GetR8L(EAX)&imm8;
    emu.cpu->SetR8L(EAX, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

Ret32Near::Ret32Near(string code_name):Instruction(code_name){

}

void Ret32Near::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetEip(InstructionHelper::Pop32(emu));
        return;
    }
    uint32_t addr;
    addr = 0x0000FFFF&InstructionHelper::Pop16(emu);
    emu.cpu->SetEip(addr);
    return;
}

Lgdt::Lgdt(string code_name):Instruction(code_name){

}

void Lgdt::Run(const Emulator& emu){
    uint16_t limit;
    uint32_t base_addr;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t m32;
        m32    = this->GetEffectiveAddr(emu);
        limit  = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(m32));
        base_addr  = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(m32+2));
        emu.cpu->SetGdtr(limit, base_addr);
        return;
    }
    uint16_t effective_addr;
    effective_addr        = this->GetEffectiveAddr(emu);
    limit                 = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr));
    base_addr             = 0x00FFFFFF&emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2));
    emu.cpu->SetGdtr(limit, base_addr);
    return;
}

Lidt::Lidt(string code_name):Instruction(code_name){

}

void Lidt::Run(const Emulator& emu){
    uint16_t limit;
    uint32_t m32, base;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        m32    = this->GetEffectiveAddr(emu);
        limit = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(m32));
        base  = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(m32+2));
        emu.cpu->SetIdtr(limit, base);
    }else{
        this->Error("Not implemented: %s::Run");
    }
    return;
}
MovR32CRX::MovR32CRX(string code_name):Instruction(code_name){

}

void MovR32CRX::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseRegIdx(emu);
    emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, emu.cpu->GetCr((CONTROL_REGISTER)this->modrm.reg_index));
    return;
}

AndEaxImm32::AndEaxImm32(string code_name):Instruction(code_name){

}

void AndEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t result;
        uint32_t eax;
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        eax = emu.cpu->GetR32(EAX);
        result = eax & imm32;
        emu.cpu->SetR32(EAX, result);
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t imm16;
    uint16_t result;
    uint16_t ax;
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    ax = emu.cpu->GetR16(EAX);
    result = ax & imm16;
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

OrRm32Imm8::OrRm32Imm8(string code_name):Instruction(code_name){

}

void OrRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8;
        uint32_t result;
        rm32 = this->GetRM32(emu);
        imm8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        result = rm32|imm8;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    uint16_t rm16;
    uint16_t imm8;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    imm8 = (int16_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    result = rm16|imm8;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

MovCRXR32::MovCRXR32(string code_name):Instruction(code_name){

}

void MovCRXR32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseRegIdx(emu);
    emu.cpu->SetCr((CONTROL_REGISTER)this->modrm.reg_index, emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm));
    return;
}

MovzxR32Rm8::MovzxR32Rm8(string code_name):Instruction(code_name){

}

void MovzxR32Rm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, (uint32_t)((uint8_t)this->GetRM8(emu)));
        return;
    }
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, (uint16_t)((uint8_t)this->GetRM8(emu)));
    return;
}

MovR32Rm32::MovR32Rm32(string code_name):Instruction(code_name){

}

void MovR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM32(emu));
        return;
    }
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM16(emu));
    return;
}

MovRm32R32::MovRm32R32(string code_name):Instruction(code_name){

}

void MovRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->SetRM32(emu, emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
        return;
    }
    this->SetRM16(emu, emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
    return;
}

SubRm32Imm8::SubRm32Imm8(string code_name):Instruction(code_name){

}

void SubRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm8;
        uint32_t rm32;
        imm8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        rm32  = this->GetRM32(emu);
        this->SetRM32(emu, rm32-imm8);
        result = (uint64_t)rm32 - (uint64_t)imm8;
        emu.cpu->UpdateEflagsForSub(result, rm32, imm8);
        return;
    }
    uint32_t result;
    uint16_t imm8;
    uint16_t rm16;
    imm8 = (int16_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    rm16  = this->GetRM16(emu);
    result = (uint32_t)rm16 - (uint32_t)imm8;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForSub16(result, rm16, imm8);
    return;
}

ImulR32Rm32Imm32::ImulR32Rm32Imm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void ImulR32Rm32Imm32::Run(const Emulator& emu){
    uint32_t rm32;
    uint32_t imm32;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        rm32 = this->GetRM32(emu);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32*imm32);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
    return;
}

SubRm32Imm32::SubRm32Imm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void SubRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t rm32;
        uint32_t imm32;
        rm32 = this->GetRM32(emu);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        result = (uint64_t)rm32 - (uint64_t)imm32;
        this->SetRM32(emu, rm32-imm32);
        emu.cpu->UpdateEflagsForSub(result, rm32, imm32);
        return;
    }
    uint32_t result;
    uint16_t rm16;
    uint16_t imm16;
    rm16 = this->GetRM16(emu);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    result = (uint32_t)rm16 - (uint32_t)imm16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForSub16(result, rm16, imm16);
    return;
}

ShrRm32Imm8::ShrRm32Imm8(string code_name):Instruction(code_name){

}

void ShrRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint8_t imm8;
        rm32 = this->GetRM32(emu);
        imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(1);
        if(imm8==0){
            return;
        }
        if(imm8==1){
            this->Error("imm8==1 is not implemented at %s::Run", this->code_name.c_str());
        }
        rm32 = rm32 >> (imm8-1);
        if(rm32&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bit op_size at %s::Run", this->code_name.c_str());
}

JmpPtr1632::JmpPtr1632(string code_name):Instruction(code_name){

}

void JmpPtr1632::Run(const Emulator& emu){
    GdtGate* gdt_gate;
    if(emu.cpu->IsProtectedMode()){
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t offset;
            uint16_t selector;
            emu.cpu->AddEip(1);
            offset = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(4);
            selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
            gdt_gate = emu.cpu->GetGdtGate(selector);
            if((gdt_gate->access_right&0x1D)==TSS_TYPE){
                emu.cpu->SaveTask(selector);
                emu.cpu->SwitchTask();
                return;
            }
            emu.cpu->SetR16(CS, selector);
            emu.cpu->SetEip(offset);
            return;
        }
        uint16_t offset;
        uint16_t selector;
        emu.cpu->AddEip(1);
        offset = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(2);
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
        gdt_gate = emu.cpu->GetGdtGate(selector);
        if((gdt_gate->access_right&0x1D)==TSS_TYPE){
            emu.cpu->SaveTask(selector);
            emu.cpu->SwitchTask();
            return;
        }
        emu.cpu->SetR16(CS, selector);
        emu.cpu->SetEip(offset);
        return;
    }
    //リアルモード処理
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32 at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t offset;
    uint16_t selector;
    emu.cpu->AddEip(1);
    offset = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->SetR16(CS, selector);
    emu.cpu->SetEip(offset);
    return;
}

PushR32::PushR32(string code_name):Instruction(code_name){

}

void PushR32::Run(const Emulator& emu){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x50);
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        InstructionHelper::Push32(emu, emu.cpu->GetR32(register_type));
        return;
    }
    InstructionHelper::Push16(emu, emu.cpu->GetR16(register_type));
    return;
}

PopR32::PopR32(string code_name):Instruction(code_name){

}

void PopR32::Run(const Emulator& emu){
    GENERAL_PURPOSE_REGISTER32 register_type;
    register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x58);
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32(register_type, InstructionHelper::Pop32(emu));
        return;
    }
    emu.cpu->SetR16(register_type, InstructionHelper::Pop16(emu));
    return;
}

PushImm8::PushImm8(string code_name):Instruction(code_name){

}

void PushImm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm8_32bit;
        imm8_32bit = (int32_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        InstructionHelper::Push32(emu, imm8_32bit);
        emu.cpu->AddEip(1);
        return;
    }
    uint16_t imm8_16bit;
    imm8_16bit = (uint16_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    InstructionHelper::Push16(emu, imm8_16bit);
    emu.cpu->AddEip(1);
    return;
}

IncR32::IncR32(string code_name):Instruction(code_name){

}

void IncR32::Run(const Emulator& emu){
    GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x40);
    uint32_t r32;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        r32 = emu.cpu->GetR32(register_type);
        emu.cpu->SetR32(register_type, r32+1);
        emu.cpu->UpdateEflagsForInc(r32+1, r32, 1);
        return;
    }
    uint16_t r16;
    uint16_t result;
    r16 = emu.cpu->GetR16(register_type);
    result = r16 + 1;
    emu.cpu->SetR16(register_type, result);
    emu.cpu->UpdateEflagsForInc16(result, r16, (uint16_t)1);
    return;
}

CmpRm32Imm32::CmpRm32Imm32(string code_name):Instruction(code_name){

}

void CmpRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm32;
        uint32_t rm32;
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        rm32  = this->GetRM32(emu);
        result = (uint64_t)rm32 - (uint64_t)imm32;
        emu.cpu->UpdateEflagsForSub(result, rm32, imm32);
        return;
    }
    uint32_t result;
    uint16_t imm16;
    uint16_t rm16;
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    rm16  = this->GetRM16(emu);
    result = (uint32_t)rm16 - (uint32_t)imm16;
    emu.cpu->UpdateEflagsForSub16(result, rm16, imm16);
    return;
}

JleRel8::JleRel8(string code_name):Instruction(code_name){

}

void JleRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(ZF) || (emu.cpu->IsFlag(OF)!=emu.cpu->IsFlag(SF))){
        emu.cpu->AddEip(rel8);
    }
    return;
}

AndRm32Imm8::AndRm32Imm8(string code_name):Instruction(code_name){

}

void AndRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm8;
        uint32_t result;
        imm8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        result = imm8 & this->GetRM32(emu);
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t imm8;
    uint16_t result;
    imm8 = (int16_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    result = imm8 & this->GetRM16(emu);
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

XorRm32R32::XorRm32R32(string code_name):Instruction(code_name){

}

void XorRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint32_t result;
        r32    = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32   = this->GetRM32(emu);
        result = rm32^r32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16^r16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
    return;
}

RorRm8Cl::RorRm8Cl(string code_name):Instruction(code_name){

}

void RorRm8Cl::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t cl;
    bool flg;
    //emu.cpu->AddEip(1);
    //this->ParseModRM(emu);
    cl  = emu.cpu->GetR8L(ECX);
    rm8 = this->GetRM8(emu);
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
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    this->SetRM8(emu, rm8);
    return;
}

PushImm32::PushImm32(string code_name):Instruction(code_name){

}

void PushImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        InstructionHelper::Push32(emu, emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(4);
        return;
    }
    InstructionHelper::Push16(emu, emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(2);
    return;
}

PushFd::PushFd(string code_name):Instruction(code_name){

}

void PushFd::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t eflgs = emu.cpu->GetEflgs();
        InstructionHelper::Push32(emu, eflgs&0x00FCFFFF);
        return;
    }
    uint16_t eflgs = emu.cpu->GetEflgs();
    InstructionHelper::Push16(emu, eflgs);
    return;
}

OutDxAl::OutDxAl(string code_name):Instruction(code_name){

}

void OutDxAl::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.io_port->Out8(emu.cpu->GetR16(EDX), emu.cpu->GetR8L(EAX));
    return;
}

CmpRm32R32::CmpRm32R32(string code_name):Instruction(code_name){

}

void CmpRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t r32;
        uint32_t rm32;
        r32    = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32   = this->GetRM32(emu);
        result = (uint64_t)rm32 - (uint64_t)r32;
        emu.cpu->UpdateEflagsForSub(result, rm32, r32);
        return;
    }
    uint32_t result;
    uint16_t r16;
    uint16_t rm16;
    r16    = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm16   = this->GetRM16(emu);
    result = (uint32_t)rm16 - (uint32_t)r16;
    emu.cpu->UpdateEflagsForSub16(result, rm16, r16);
    return;
}

ShrRm8Imm8::ShrRm8Imm8(string code_name):Instruction(code_name){

}

void ShrRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    rm8  = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    if(imm8==0){
        return;
    }
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    rm8 = rm8 >> (imm8-1);
    if(rm8&0x01){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm8 = rm8 >> 1;
    this->SetRM8(emu, rm8);
    emu.cpu->UpdateEflagsForShr(rm8);
}

LeaR32M::LeaR32M(string code_name):Instruction(code_name){

}

//この機械語命令はオペランドサイズとアドレスサイズを考慮する必要があります。
void LeaR32M::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if((!emu.cpu->Is32bitsMode()) ^ emu.cpu->IsPrefixAddrSize()){//16bit addr_size
            this->Error("Not implemented: addr_size=16 at %s::Run", this->code_name.c_str());
        }
        uint32_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, effective_addr);
        return;
    }else{
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bit addr_size
            this->Error("Not implemented: addr_size=32 at %s::Run", this->code_name.c_str());
        }
        uint16_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, effective_addr);
    }
    return;
}

PopFd::PopFd(string code_name):Instruction(code_name){

}

void PopFd::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t eflgs;
        eflgs = InstructionHelper::Pop32(emu);
        emu.cpu->SetEflgs(eflgs);
        return;
    }
    uint32_t eflgs;
    eflgs = InstructionHelper::Pop16(emu);
    emu.cpu->SetEflgs((emu.cpu->GetEflgs()&0xFFFF0000)|eflgs);
    return;
}

Leave::Leave(string code_name):Instruction(code_name){

}

void Leave::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32(ESP, emu.cpu->GetR32(EBP));
        emu.cpu->SetR32(EBP, InstructionHelper::Pop32(emu));
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CmpR32Rm32::CmpR32Rm32(string code_name):Instruction(code_name){

}

void CmpR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t rm32;
        uint64_t result;
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(emu);
        result = (uint64_t)r32 - (uint64_t)rm32;
        emu.cpu->UpdateEflagsForSub(result, r32, rm32);
        return;
    }
    uint16_t r16;
    uint16_t rm16;
    uint32_t result;
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm16 = this->GetRM16(emu);
    result = (uint32_t)r16 - (uint32_t)rm16;
    emu.cpu->UpdateEflagsForSub16(result, r16, rm16);
    return;
}

JgRel8::JgRel8(string code_name):Instruction(code_name){

}

void JgRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if((!emu.cpu->IsFlag(ZF)) && (emu.cpu->IsFlag(SF)==emu.cpu->IsFlag(OF))){
        emu.cpu->AddEip(rel8);
    }
    return;
}

ImulR32Rm32::ImulR32Rm32(string code_name):Instruction(code_name){

}

void ImulR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32*r32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    return;
}

MovsxR32Rm16::MovsxR32Rm16(string code_name):Instruction(code_name){

}

void MovsxR32Rm16::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint32_t rm16;
    rm16 = (int32_t)(int16_t)this->GetRM16(emu);
    emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
    return;
}

PushRm32::PushRm32(string code_name):Instruction(code_name){

}

void PushRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        InstructionHelper::Push32(emu, this->GetRM32(emu));
        return;
    }
    InstructionHelper::Push16(emu, this->GetRM16(emu));
    return;
}

IncRm32::IncRm32(string code_name):Instruction(code_name){

}

void IncRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(emu);
        this->SetRM32(emu, rm32+1);
        emu.cpu->UpdateEflagsForInc(rm32+1, rm32, 1);
        return;
    }
    uint16_t rm16;
    uint16_t result;
    uint16_t d = 1;
    rm16 = this->GetRM16(emu);
    result = rm16+d;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForInc16(result, rm16, d);
    return;
}

DecR32::DecR32(string code_name):Instruction(code_name){

}

void DecR32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)(emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-0x48);
        emu.cpu->AddEip(1);
        r32 = emu.cpu->GetR32(register_type);
        result = r32 - 1;
        emu.cpu->SetR32(register_type, result);
        emu.cpu->UpdateEflagsForDec(result, r32, (uint32_t)0xFFFFFFFF);
        return;
    }
    uint16_t r16;
    uint16_t result;
    GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)(emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-0x48);
    emu.cpu->AddEip(1);
    r16 = emu.cpu->GetR16(register_type);
    result = r16 - 1;
    emu.cpu->SetR16(register_type, result);
    emu.cpu->UpdateEflagsForDec(result, r16, (uint16_t)0xFFFF);
    return;
}

TestRm8R8::TestRm8R8(string code_name):Instruction(code_name){

}

void TestRm8R8::Run(const Emulator& emu){
    uint8_t r8;
    uint8_t rm8;
    uint8_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8(this->modrm.reg_index); 
    result = rm8 & r8;
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

JnsRel8::JnsRel8(string code_name):Instruction(code_name){

}

void JnsRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsFlag(SF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

CmpRm32Imm8::CmpRm32Imm8(string code_name):Instruction(code_name){

}

void CmpRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm8;
        uint32_t rm32;
        imm8 = (int32_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(1);
        rm32  = this->GetRM32(emu);
        result = (uint64_t)rm32 - (uint64_t)imm8;
        emu.cpu->UpdateEflagsForSub(result, rm32, imm8);
        return;
    }
    uint16_t imm8;
    uint16_t rm16;
    uint32_t result;
    imm8 = (int16_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    rm16  = this->GetRM16(emu);
    result = (uint32_t)rm16 - (uint32_t)imm8;
    emu.cpu->UpdateEflagsForSub16(result, rm16, imm8);
    return;
}

AndRm32Imm32::AndRm32Imm32(string code_name):Instruction(code_name){

}

void AndRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm32;
        uint32_t result;
        uint32_t rm32;
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        rm32 = this->GetRM32(emu);
        result = rm32 & imm32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t imm16;
    uint16_t result;
    uint16_t rm16;
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    rm16 = this->GetRM16(emu);
    result = rm16 & imm16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

SalRm32Imm8::SalRm32Imm8(string code_name):Instruction(code_name){

}

void SalRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm8;
        rm32 = this->GetRM32(emu);
        imm8 = (uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(1);
        if(imm8==0){
            return;
        }
        if(imm8==1){
            this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
        }
        rm32 = rm32 << (imm8-1);
        if(rm32&0x80000000){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 << 1;
        this->SetRM32(emu, rm32);
        return;
    }
    uint16_t rm16;
    uint16_t imm8;
    rm16 = this->GetRM16(emu);
    imm8 = (uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    if(imm8==0){
        return;
    }
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    rm16 = rm16 << (imm8-1);
    if(rm16&0x8000){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm16 = rm16 << 1;
    this->SetRM16(emu, rm16);
    return;
}

MovsxR32Rm8::MovsxR32Rm8(string code_name):Instruction(code_name){

}

void MovsxR32Rm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm8;
        rm8 = (int32_t)(int8_t)this->GetRM8(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
        return;
    }
    uint16_t rm8;
    rm8 = (int16_t)(int8_t)this->GetRM8(emu);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
    return;
}

AndR8Rm8::AndR8Rm8(string code_name):Instruction(code_name){

}

void AndR8Rm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    result = emu.cpu->GetR8(this->modrm.reg_index) & this->GetRM8(emu);
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);
}

XchgR32Rm32::XchgR32Rm32(string code_name):Instruction(code_name){

}

void XchgR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32);
        return;
    }
    uint32_t rm16;
    uint32_t r16;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    this->SetRM16(emu, r16);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
}

TestRm32R32::TestRm32R32(string code_name):Instruction(code_name){

}

void TestRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t rm32;
        uint32_t result;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index); 
        result = rm32 & r32;
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t r16;
    uint16_t rm16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index); 
    result = rm16 & r16;
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

JsRel8::JsRel8(string code_name):Instruction(code_name){

}

void JsRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(SF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

DivRm32::DivRm32(string code_name):Instruction(code_name){

}

void DivRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint64_t r64;
        uint32_t edx, eax;
        edx = emu.cpu->GetR32(EDX);
        eax = emu.cpu->GetR32(EAX);
        rm32 = this->GetRM32(emu);

        r64  = (((uint64_t)edx)<<((uint64_t)32))| ((uint64_t)eax);
        emu.cpu->SetR32(EAX, r64/((uint64_t)rm32));
        emu.cpu->SetR32(EDX, r64%(uint64_t)rm32);
        return;
    }
    uint16_t rm16;
    uint32_t r32;
    uint16_t dx, ax;
    dx = emu.cpu->GetR16(EDX);
    ax = emu.cpu->GetR16(EAX);
    rm16 = this->GetRM16(emu);
    r32  = (((uint32_t)dx)<<((uint32_t)16))| ((uint32_t)ax);
    emu.cpu->SetR16(EAX, r32/((uint32_t)rm16));
    emu.cpu->SetR16(EDX, r32%(uint32_t)rm16);
    return;
}

NotRm32::NotRm32(string code_name):Instruction(code_name){

}

void NotRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(emu);
        rm32 = ~rm32;
        this->SetRM32(emu, rm32);
        return;
    }
    uint16_t rm16;
    rm16 = this->GetRM16(emu);
    rm16 = ~rm16;
    this->SetRM16(emu, rm16);
    return;
}

JgeRel8::JgeRel8(string code_name):Instruction(code_name){

}

void JgeRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(SF)==emu.cpu->IsFlag(OF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

SubRm32R32::SubRm32R32(string code_name):Instruction(code_name){

}

void SubRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = (uint64_t)rm32 - (uint64_t)r32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForSub(result, rm32, r32);
        return;
    }
    uint32_t result;
    uint16_t rm16;
    uint16_t r16;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint32_t)rm16 - (uint32_t)r16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForSub16(result, rm16, r16);
    return;
}

SarRm32Imm8::SarRm32Imm8(string code_name):Instruction(code_name){

}

void SarRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        int32_t rm32;//符号付回転なので、in32_tにしている。
        uint32_t imm8;
        rm32 = this->GetRM32(emu);
        imm8 = (uint32_t)(emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        if(imm8==0){
            return;
        }
        if(imm8==1){
            this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
        }
        rm32 = rm32 >> (imm8-1);
        if(rm32&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);//shrと同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

OrRm32R32::OrRm32R32(string code_name):Instruction(code_name){

}

void OrRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t r32;
        uint32_t result;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = rm32 | r32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16 | r16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

Cdq::Cdq(string code_name):Instruction(code_name){

}

void Cdq::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        bool sign_flg = (emu.cpu->GetR32(EAX)&SIGN_FLG4)?true:false;
        if(sign_flg){
            emu.cpu->SetR32(EDX, 0xFFFFFFFF);
        }else{
            emu.cpu->SetR32(EDX, 0x00000000);
        }
        return;
    }
    bool sign_flg = (emu.cpu->GetR16(EAX)&SIGN_FLG2)?true:false;
    if(sign_flg){
        emu.cpu->SetR16(EDX, 0xFFFF);
    }else{
        emu.cpu->SetR16(EDX, 0x0000);
    }
    return;
}

IdivRm32::IdivRm32(string code_name):Instruction(code_name){

}

void IdivRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        int64_t rm32;
        int64_t r64;
        int32_t edx, eax;
        edx = emu.cpu->GetR32(EDX);
        eax = emu.cpu->GetR32(EAX);
        rm32 = (int64_t)(int32_t)this->GetRM32(emu);
        r64  = (((int64_t)edx)<<((int64_t)32))| ((int64_t)eax);
        emu.cpu->SetR32(EAX, r64/rm32);
        emu.cpu->SetR32(EDX, r64%rm32);
        return;
    }
    int32_t rm16;
    int32_t r32;
    int16_t dx, ax;
    dx = emu.cpu->GetR16(EDX);
    ax = emu.cpu->GetR16(EAX);
    rm16 = (int32_t)(int16_t)this->GetRM16(emu);
    r32  = (((int32_t)dx)<<((int32_t)32))| ((int32_t)ax);
    emu.cpu->SetR16(EAX, r32/rm16);
    emu.cpu->SetR16(EDX, r32%rm16);
    return;
}

JlRel8::JlRel8(string code_name):Instruction(code_name){

}

void JlRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(SF)!=emu.cpu->IsFlag(OF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

Sti::Sti(string code_name):Instruction(code_name){

}

void Sti::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.cpu->SetFlag(IF);
    return;
}

PushEs::PushEs(string code_name):Instruction(code_name){

}

void PushEs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t es = 0;
        emu.cpu->AddEip(1);
        es = (uint32_t)emu.cpu->GetR16(ES);
        InstructionHelper::Push32(emu, es);
        return;
    }
    uint16_t es;
    emu.cpu->AddEip(1);
    es = emu.cpu->GetR16(ES);
    InstructionHelper::Push16(emu, es);
    return;
}

PushDs::PushDs(string code_name):Instruction(code_name){

}

void PushDs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t ds = 0;
        emu.cpu->AddEip(1);
        ds = (uint32_t)emu.cpu->GetR16(DS);
        InstructionHelper::Push32(emu, ds);
        return;
    }
    uint16_t ds;
    emu.cpu->AddEip(1);
    ds = emu.cpu->GetR16(DS);
    InstructionHelper::Push16(emu, ds);
    return;
}

PushCs::PushCs(string code_name):Instruction(code_name){

}

void PushCs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t cs = 0;
        emu.cpu->AddEip(1);
        cs = (uint32_t)emu.cpu->GetR16(CS);
        InstructionHelper::Push32(emu, cs);
        return;
    }
    uint16_t cs;
    emu.cpu->AddEip(1);
    cs = emu.cpu->GetR16(CS);
    InstructionHelper::Push16(emu, cs);
    return;
}

PushAd::PushAd(string code_name):Instruction(code_name){

}

void PushAd::Run(const Emulator& emu){
    uint32_t esp;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        esp = emu.cpu->GetR32(ESP);
        InstructionHelper::Push32(emu, emu.cpu->GetR32(EAX));
        InstructionHelper::Push32(emu, emu.cpu->GetR32(ECX));
        InstructionHelper::Push32(emu, emu.cpu->GetR32(EDX));
        InstructionHelper::Push32(emu, emu.cpu->GetR32(EBX));
        InstructionHelper::Push32(emu, esp);
        InstructionHelper::Push32(emu, emu.cpu->GetR32(EBP));
        InstructionHelper::Push32(emu, emu.cpu->GetR32(ESI));
        InstructionHelper::Push32(emu, emu.cpu->GetR32(EDI));
        return;
    }
    uint16_t sp;
    sp = emu.cpu->GetR16(ESP);
    InstructionHelper::Push16(emu, emu.cpu->GetR16(EAX));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(ECX));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(EDX));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(EBX));
    InstructionHelper::Push16(emu, sp);
    InstructionHelper::Push16(emu, emu.cpu->GetR16(EBP));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(ESI));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(EDI));
    return;
}

InAlDx::InAlDx(string code_name):Instruction(code_name){

}

void InAlDx::Run(const Emulator& emu){
    uint8_t al;
    uint32_t dx;
    emu.cpu->AddEip(1);
    dx = (uint32_t)emu.cpu->GetR16(EDX);
    emu.cpu->SetR8(EAX, emu.io_port->In8(dx));
    return;
}

DecRm32::DecRm32(string code_name):Instruction(code_name){

}

void DecRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t result;
        rm32 = this->GetRM32(emu);
        result = rm32 - 1;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForDec(result, rm32, (uint32_t)0xFFFFFFFF);
        return;
    }
    uint16_t rm16;
    uint16_t result;
    uint16_t d = 0xFFFF;
    rm16 = this->GetRM16(emu);
    result = rm16+d;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForDec(result, rm16, d);
    return;
}

JaRel8::JaRel8(string code_name):Instruction(code_name){

}

void JaRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if((!emu.cpu->IsFlag(CF))&&(!emu.cpu->IsFlag(ZF))){
        emu.cpu->AddEip(rel8);
    }
    return;
}

PopAd::PopAd(string code_name):Instruction(code_name){

}

void PopAd::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        edi = InstructionHelper::Pop32(emu);
        esi = InstructionHelper::Pop32(emu);
        ebp = InstructionHelper::Pop32(emu);
        esp = InstructionHelper::Pop32(emu);
        ebx = InstructionHelper::Pop32(emu);
        edx = InstructionHelper::Pop32(emu);
        ecx = InstructionHelper::Pop32(emu);
        eax = InstructionHelper::Pop32(emu);

        emu.cpu->SetR32(EAX, eax);
        emu.cpu->SetR32(ECX, ecx);
        emu.cpu->SetR32(EDX, edx);
        emu.cpu->SetR32(EBX, ebx);
        //espは無視する
        emu.cpu->SetR32(EBP, ebp);
        emu.cpu->SetR32(ESI, esi);
        emu.cpu->SetR32(EDI, edi);
        return;
    }else{
        uint16_t ax, cx, dx, bx, sp, bp, si, di;
        di = InstructionHelper::Pop16(emu);
        si = InstructionHelper::Pop16(emu);
        bp = InstructionHelper::Pop16(emu);
        sp = InstructionHelper::Pop16(emu);
        bx = InstructionHelper::Pop16(emu);
        dx = InstructionHelper::Pop16(emu);
        cx = InstructionHelper::Pop16(emu);
        ax = InstructionHelper::Pop16(emu);

        emu.cpu->SetR16(EAX, ax);
        emu.cpu->SetR16(ECX, cx);
        emu.cpu->SetR16(EDX, dx);
        emu.cpu->SetR16(EBX, bx);
        //espは無視する
        emu.cpu->SetR16(EBP, bp);
        emu.cpu->SetR16(ESI, si);
        emu.cpu->SetR16(EDI, di);
    }
    return;
}

PopDs::PopDs(string code_name):Instruction(code_name){

}

void PopDs::Run(const Emulator& emu){
    uint32_t ds;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        ds = InstructionHelper::Pop32(emu);
        emu.cpu->SetR16(DS, ds);
        return;
    }
    ds = InstructionHelper::Pop16(emu);
    emu.cpu->SetR16(DS, ds);
    return;
}

PopEs::PopEs(string code_name):Instruction(code_name){

}

void PopEs::Run(const Emulator& emu){
    uint32_t es = 0;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        es = InstructionHelper::Pop32(emu);
        emu.cpu->SetR16(ES, es);
        return;
    }
    es = InstructionHelper::Pop16(emu);
    emu.cpu->SetR16(ES, es);
    return;
}

Iretd::Iretd(string code_name):Instruction(code_name){

}

void Iretd::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsProtectedMode()){
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
        }else{
            uint16_t cs, ip, eflags;
            ip = InstructionHelper::Pop16(emu);
            cs = InstructionHelper::Pop16(emu);
            eflags = InstructionHelper::Pop16(emu);
            emu.cpu->SetEip(ip);
            emu.cpu->SetR16(CS, cs);
            emu.cpu->SetEflgs(eflags);
        }
        return;
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        bool outer_privilege_level;
        uint16_t cs, ss;
        uint32_t eip, eflags, esp;
        uint8_t cpl, rpl;
        eip    = InstructionHelper::Pop32(emu);
        cs     = InstructionHelper::Pop32(emu);
        eflags = InstructionHelper::Pop32(emu);
        rpl    = CpuHelper::GetRpl(cs);//呼び出し元特権レベル
        cpl    = emu.cpu->GetCpl();
        if(rpl>cpl){
            esp = InstructionHelper::Pop32(emu);
            ss = InstructionHelper::Pop32(emu);
            emu.cpu->SetR32(ESP, esp);
            emu.cpu->SetR16(SS, ss);
            emu.cpu->SetR16(CS, cs);
            emu.cpu->SetEip(eip);
            emu.cpu->SetEflgs(eflags);
            SEGMENT_REGISTER array[] = {ES, FS, GS, DS};
            for(auto segment_register_type:array){
                uint16_t selector = emu.cpu->GetR16(segment_register_type);
                GdtGate* gdt_gate = emu.cpu->GetGdtGate(selector);
                uint8_t dpl = CpuHelper::GetDpl(gdt_gate->access_right);
                if(emu.cpu->GetCpl()>dpl){
                    emu.cpu->SetR16(segment_register_type, 0);
                }
            }
        }else if(rpl==cpl){//rpl<=cpl
            emu.cpu->SetR16(CS, cs);
            emu.cpu->SetEip(eip);
            emu.cpu->SetEflgs(eflags);
        }else{
            this->Error("Not implemented: rpl<cpl %s::Run", this->code_name.c_str());
        }
        return;
    }else{
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
    }
    return;
}

MovEaxMoffs32::MovEaxMoffs32(string code_name):Instruction(code_name){

}

void MovEaxMoffs32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32(EAX, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess()))));
        emu.cpu->AddEip(4);
        return;
    }else{
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bit_addr
            this->Error("Not implemented: addr_size=32bit at %s::Run", this->code_name.c_str());
        }else{//16bit_addr
            emu.cpu->SetR16(EAX, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()))));
            emu.cpu->AddEip(2);
        }
    }
    return;
}

MovMoffs32Eax::MovMoffs32Eax(string code_name):Instruction(code_name){

}

void MovMoffs32Eax::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    //op_size = 32bit
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bit_addr
            emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess())), emu.cpu->GetR32(EAX));
            emu.cpu->AddEip(4);
        }else{//16bit_addr
            emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess())), emu.cpu->GetR32(EAX));
            emu.cpu->AddEip(2);
        }
        return;
    }
    //op_size = 16bit
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bit_addr
        emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess())), emu.cpu->GetR16(EAX));
        emu.cpu->AddEip(4);
    }else{//16bit_addr
        emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess())), emu.cpu->GetR16(EAX));
        emu.cpu->AddEip(2);
    }
}

SubR32Rm32::SubR32Rm32(string code_name):Instruction(code_name){

}

void SubR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t rm32;
        uint32_t r32;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        result = (uint64_t)r32 - (uint64_t)rm32;
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        emu.cpu->UpdateEflagsForSub(result, r32, rm32);
        return;
    }
    uint32_t result;
    uint16_t rm16;
    uint16_t r16;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint32_t)r16 - (uint32_t)rm16;
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForSub16(result, r16, rm16);
    return;
}

DecRm8::DecRm8(string code_name):Instruction(code_name){

}

void DecRm8::Run(const Emulator& emu){
    uint8_t r8;
    uint8_t result;
    uint8_t d = 0xFF;
    r8 = this->GetRM8(emu);
    result = r8 - 1;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForDec(result, r8, d);
}

OrRm32Imm32::OrRm32Imm32(string code_name):Instruction(code_name){

}

void OrRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t result;
        result = this->GetRM32(emu)|emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);  
        return;
    }
    uint16_t result;
    result = this->GetRM16(emu)|emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);  
    return;
}

NegRm32::NegRm32(string code_name):Instruction(code_name){

}

void NegRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t result;
        uint32_t rm32;
        uint32_t zero = 0;
        rm32 = this->GetRM32(emu);
        if(rm32==0){
            emu.cpu->ClearFlag(CF);
        }else{
            emu.cpu->SetFlag(CF);
        }
        result = (uint64_t)zero - (uint64_t)rm32;
        this->SetRM32(emu, zero-rm32);
        emu.cpu->UpdateEflagsForDec(result, zero, rm32);
        return;
    }
    uint16_t result;
    uint16_t rm16;
    uint16_t zero = 0;
    rm16 = this->GetRM16(emu);
    if(rm16==0){
        emu.cpu->ClearFlag(CF);
    }else{
        emu.cpu->SetFlag(CF);
    }
    result = zero - rm16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForDec(result, zero, rm16);
    return;
}

TestEaxImm32::TestEaxImm32(string code_name):Instruction(code_name){

}

void TestEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->UpdateEflagsForAnd(emu.cpu->GetR32(EAX)&emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(4);
        return;
    }
    uint16_t ax = emu.cpu->GetR16(EAX);
    uint16_t imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    uint16_t result = ax&imm16;
    emu.cpu->UpdateEflagsForAnd(result);
    emu.cpu->AddEip(2);
    return;
}

OrEaxImm32::OrEaxImm32(string code_name):Instruction(code_name){

}

void OrEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t result;
        result = emu.cpu->GetR32(EAX) | emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        emu.cpu->SetR32(EAX, result);
        emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
        return;
    }
    uint16_t result;
    result = emu.cpu->GetR16(EAX) | emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

AddRm32Imm32::AddRm32Imm32(string code_name):Instruction(code_name){

}

void AddRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm32;
        uint64_t result;
        rm32  = this->GetRM32(emu);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        result = rm32+imm32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAdd(result, rm32, imm32);
        return;
    }
    uint32_t result;
    uint16_t rm16;
    uint16_t imm16;
    rm16 = this->GetRM16(emu);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    result = (uint32_t)rm16 + (uint32_t)imm16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm16, imm16);
    return;
}

XorRm32Imm8::XorRm32Imm8(string code_name):Instruction(code_name){

}

void XorRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm8;
        uint32_t result;
        imm8   =  (int32_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(1);
        result = this->GetRM32(emu)^imm8;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SubEaxImm32::SubEaxImm32(string code_name):Instruction(code_name){

}

//フラグレジスタの更新が未実装
void SubEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t eax;
        uint32_t imm32;
        eax = emu.cpu->GetR32(EAX);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        result = (uint64_t)eax - (uint64_t)imm32;
        emu.cpu->SetR32(EAX, result);
        emu.cpu->UpdateEflagsForSub(result, eax, imm32);
        return;
    }
    uint32_t result;
    uint16_t ax;
    uint16_t imm16;
    ax = emu.cpu->GetR16(EAX);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    result = (uint32_t)ax - (uint32_t)imm16;
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForSub16(result, ax, imm16);
    return;
}

ImulR32Rm32Imm8::ImulR32Rm32Imm8(string code_name):Instruction(code_name){

}

void ImulR32Rm32Imm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t imm8 = (int32_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, this->GetRM32(emu)*imm8);
        emu.cpu->AddEip(1);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetneRm8::SetneRm8(string code_name):Instruction(code_name){

}

void SetneRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if(!emu.cpu->IsFlag(ZF)){
            this->SetRM8(emu, 1);
        }else{
            this->SetRM8(emu, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SubRm8R8::SubRm8R8(string code_name):Instruction(code_name){

}

void SubRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8, r8;
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t result = (uint32_t)rm8- (uint32_t)r8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForSub8(result, rm8, r8);
    return;
}

AdcRm8R8::AdcRm8R8(string code_name):Instruction(code_name){

}

void AdcRm8R8::Run(const Emulator& emu){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cf  = emu.cpu->IsFlag(CF)?1:0;
    result = (uint16_t)rm8+(uint16_t)r8+(uint16_t)cf;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAdd((uint16_t)result, (uint8_t)rm8, (uint8_t)(r8+cf));
    return;
}

Clc::Clc(string code_name):Instruction(code_name){

}

void Clc::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.cpu->ClearFlag(CF);
}

AddR8Rm8::AddR8Rm8(string code_name):Instruction(code_name){

}

void AddR8Rm8::Run(const Emulator& emu){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint16_t)rm8+(uint16_t)r8;
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAdd(result, rm8, r8);
}

SbbRm8R8::SbbRm8R8(string code_name):Instruction(code_name){

}

void SbbRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8, r8;
    uint8_t cf = emu.cpu->IsFlag(CF)?1:0;
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t result = (uint32_t)rm8- ((uint32_t)r8+cf);
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForSub8(result, rm8, r8+cf);
}

ShrRm32Cl::ShrRm32Cl(string code_name):Instruction(code_name){

}

void ShrRm32Cl::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint8_t cl;
        rm32 = this->GetRM32(emu);
        cl = emu.cpu->GetR8L(ECX);
        if(cl==0){
            return;
        }
        if(cl==1){
            if(rm32&SIGN_FLG4){
                emu.cpu->SetFlag(OF);
            }else{
                emu.cpu->ClearFlag(OF);
            }
        }
        rm32 = rm32 >> (cl-1);
        if(rm32&1){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);
        return;
    }
    uint16_t rm16;
    uint8_t cl;
    rm16 = this->GetRM16(emu);
    cl = emu.cpu->GetR8L(ECX);
    if(cl==0){//cl==0の時、何もしない。
        return;
    }
    if(cl==1){
        if(rm16&SIGN_FLG2){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
    }
    for(uint32_t i=0; i<cl; i++){
        if(rm16&1){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm16 = rm16 >> 1;
    }
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);
    return;
}

Xlatb::Xlatb(string code_name):Instruction(code_name){

}

void Xlatb::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//xlatbはaddr_sizeで判別
        uint32_t al;
        al = (uint32_t)emu.cpu->GetR8L(EAX);
        emu.cpu->SetR8L(EAX, emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(emu.cpu->GetR32(EBX)))+al);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CmpRm8R8::CmpRm8R8(string code_name):Instruction(code_name){

}

void CmpRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t r8;
    uint8_t rm8;
    uint32_t result;
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(emu);
    result = (uint32_t)rm8 - (uint32_t)r8;
    emu.cpu->UpdateEflagsForSub8(result, rm8, r8);
}

CmpEaxImm32::CmpEaxImm32(string code_name):Instruction(code_name){

}

void CmpEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t result;
        uint32_t imm32;
        uint32_t eax;
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        eax  = emu.cpu->GetR32(EAX);
        result = (uint64_t)eax - (uint64_t)imm32;
        emu.cpu->UpdateEflagsForSub(result, eax, imm32);
        return;
    }
    uint32_t result;
    uint16_t imm16;
    uint16_t ax;
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    ax  = emu.cpu->GetR16(EAX);
    result = (uint32_t)ax - (uint32_t)imm16;
    emu.cpu->UpdateEflagsForSub16(result, ax, imm16);
    return;
}

TestRm32Imm32::TestRm32Imm32(string code_name):Instruction(code_name){

}

void TestRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t imm32;
        rm32 = this->GetRM32(emu);
        imm32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        emu.cpu->UpdateEflagsForAnd(rm32&imm32);
        return;
    }
    uint16_t rm16;
    uint16_t imm16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    result = rm16&imm16;
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

LtrRm16::LtrRm16(string code_name):Instruction(code_name){

}

void LtrRm16::Run(const Emulator& emu){
    emu.cpu->SetTr(this->GetRM16(emu));
    return;
}

AndRm8R8::AndRm8R8(string code_name):Instruction(code_name){

}

void AndRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8, r8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    r8 = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8&r8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

JmpM1632::JmpM1632(string code_name):Instruction(code_name){

}

//Farジャンプはプロテクトモードかリアルモードかで挙動が変わる。
//プロテクトモードな
void JmpM1632::Run(const Emulator& emu){
    if(!emu.cpu->IsProtectedMode()){//real mode
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            this->Error("Not implemented: op_size=32bit on real mode at %s::Run");
        }
        uint32_t addr;
        uint16_t offset_addr;
        uint16_t selector;
        addr = this->GetEffectiveAddr(emu);
        offset_addr = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr));
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr+2));
        emu.cpu->SetR16(CS, selector);
        emu.cpu->SetEip((uint32_t)offset_addr);
        return;
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t addr;
        uint32_t offset_addr;
        uint16_t selector;
        addr = this->GetEffectiveAddr(emu);
        offset_addr = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(addr));
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(addr+4));
        GdtGate* gdt_gate = emu.cpu->GetGdtGate(selector);
        if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
            emu.cpu->SetR16(CS, selector);
            emu.cpu->SetEip(offset_addr);
        }else{
            emu.cpu->SaveTask(selector);
            emu.cpu->SwitchTask();
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SeteRm8::SeteRm8(string code_name):Instruction(code_name){

}

void SeteRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if(emu.cpu->IsFlag(ZF)){
            this->SetRM8(emu, 1);
        }else{
            this->SetRM8(emu, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

MovAlMoffs8::MovAlMoffs8(string code_name):Instruction(code_name){

}

void MovAlMoffs8::Run(const Emulator& emu){
    uint32_t offset32;
    uint8_t data;
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//32bit_addr
        offset32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        data     = emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(offset32));
        emu.cpu->SetR8L(EAX, data);
        return;
    }
    emu.cpu->SetR8L(EAX, emu.mem->Read8(emu.cpu->GetLinearAddrForDataAccess(emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()))));
    emu.cpu->AddEip(2);
    return;
}

RcrRm8Imm8::RcrRm8Imm8(string code_name):Instruction(code_name){

}

void RcrRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    rm8 = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    if(imm8==0){
        return;
    }
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    for(uint32_t i=0; i<imm8; i++){
        if(rm8&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm8 = rm8 >> 1;
        rm8 = rm8 | (emu.cpu->IsFlag(CF)?0x80:0x00);
    }
    this->SetRM8(emu, rm8);
    return;
}

SarRm8Imm8::SarRm8Imm8(string code_name):Instruction(code_name){

}

void SarRm8Imm8::Run(const Emulator& emu){
    int8_t rm8;//符号付回転なので、int8_t
    uint8_t imm8;
    rm8 = this->GetRM8(emu);
    imm8 = (uint8_t)(emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    bool flg = (rm8&SIGN_FLG1)? 1:0;
    if(imm8==0){
        return;
    }
    if(imm8==1){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    rm8 = rm8 >> (imm8-1);
    if(rm8&0x01){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm8 = rm8 >> 1;
    this->SetRM8(emu, rm8);
    emu.cpu->UpdateEflagsForShr(rm8);
    return;
}

Cld::Cld(string code_name):Instruction(code_name){

}

void Cld::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.cpu->ClearFlag(DF);
    return;
}

CmpsM8M8::CmpsM8M8(string code_name):Instruction(code_name){

}

void CmpsM8M8::Run(const Emulator& emu){
    //this->Error("Not implemented: at %s::Run", this->code_name.c_str());
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment_override at %s::Run", this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){//アドレスで場合分け
        uint32_t base_ds, base_es;
        uint32_t esi, edi;
        uint32_t base_ds_esi, base_es_edi;
        uint8_t m1, m2;
        uint32_t result;
        uint32_t d;
        base_ds = emu.cpu->GetBaseAddr(DS);
        base_es = emu.cpu->GetBaseAddr(ES);
        esi     = emu.cpu->GetR32(ESI);
        edi     = emu.cpu->GetR32(EDI);
        base_ds_esi = base_ds+esi;
        base_es_edi = base_es+edi;
        m1      = emu.mem->Read8(base_ds_esi);
        m2      = emu.mem->Read8(base_es_edi);
        result = (uint32_t)m1 - (uint32_t)m2;
        emu.cpu->UpdateEflagsForSub8(result, m1, m2);
        d = emu.cpu->IsFlag(DF)? -1:1;
        emu.cpu->SetR32(ESI, esi+d);
        emu.cpu->SetR32(EDI, edi+d);
        return;
    }
    uint32_t base_ds, base_es;
    uint16_t si, di;
    uint32_t base_ds_si, base_es_di;
    uint8_t m1, m2;
    uint32_t result;
    uint16_t d;
    base_ds = emu.cpu->GetBaseAddr(DS);
    base_es = emu.cpu->GetBaseAddr(ES);
    si     = emu.cpu->GetR16(ESI);
    di     = emu.cpu->GetR16(EDI);
    base_ds_si = base_ds+si;
    base_es_di = base_es+di;
    m1      = emu.mem->Read8(base_ds_si);
    m2      = emu.mem->Read8(base_es_di);
    result = (uint32_t)m1 - (uint32_t)m2;
    emu.cpu->UpdateEflagsForSub8(result, m1, m2);
    d = emu.cpu->IsFlag(DF)? -1:1;
    emu.cpu->SetR16(ESI, si+d);
    emu.cpu->SetR16(EDI, di+d);
    return;
}

CmpsM32M32::CmpsM32M32(string code_name):Instruction(code_name){

}

void CmpsM32M32::Run(const Emulator& emu){
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment_override at %s::Run", this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    uint32_t base_ds, base_es;
    uint32_t esi, edi;
    uint32_t base_ds_esi, base_es_edi;
    uint32_t d;//オペランドサイズで決まる
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    base_ds = emu.cpu->GetBaseAddr(DS);
    base_es = emu.cpu->GetBaseAddr(ES);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        esi     = emu.cpu->GetR32(ESI);
        edi     = emu.cpu->GetR32(EDI);
    }else{
        esi     = emu.cpu->GetR16(ESI);
        edi     = emu.cpu->GetR16(EDI);
    }
    base_ds_esi = base_ds+esi;
    base_es_edi = base_es+edi;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t m1, m2;
        uint64_t result;
        m1      = emu.mem->Read32(emu.cpu->GetPhysicalAddr(base_ds_esi));
        m2      = emu.mem->Read32(emu.cpu->GetPhysicalAddr(base_es_edi));
        result = (uint64_t)m1 - (uint64_t)m2;
        emu.cpu->UpdateEflagsForSub(result, m1, m2);
    }else{
        uint16_t m1, m2;
        uint32_t result;
        m1      = emu.mem->Read32(base_ds_esi);
        m2      = emu.mem->Read32(base_es_edi);
        result = (uint64_t)m1 - (uint64_t)m2;
        emu.cpu->UpdateEflagsForSub16(result, m1, m2);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        emu.cpu->SetR32(ESI, esi+d);
        emu.cpu->SetR32(EDI, edi+d);
    }else{
        emu.cpu->SetR16(ESI, esi+d);
        emu.cpu->SetR16(EDI, edi+d);
    }
    return;
}


SetaRm8::SetaRm8(string code_name):Instruction(code_name){

}

void SetaRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if((!emu.cpu->IsFlag(ZF))&&(!emu.cpu->IsFlag(CF))){
            this->SetRM8(emu, 1);
        }else{
            this->SetRM8(emu, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SetbRm8::SetbRm8(string code_name):Instruction(code_name){

}

void SetbRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if(emu.cpu->IsFlag(CF)){
            this->SetRM8(emu, 1);
        }else{
            this->SetRM8(emu, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

TestRm8Imm8::TestRm8Imm8(string code_name):Instruction(code_name){

}

void TestRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    uint8_t result;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    rm8  = this->GetRM8(emu);
    result = rm8 & imm8;
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

MovzxR32Rm16::MovzxR32Rm16(string code_name):Instruction(code_name){

}

void MovzxR32Rm16::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint32_t rm16 = (uint32_t)this->GetRM16(emu);
    emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
}

SbbR8Rm8::SbbR8Rm8(string code_name):Instruction(code_name){

}

void SbbR8Rm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t r8, rm8;
    uint8_t cf = emu.cpu->IsFlag(CF)?1:0;
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(emu);
    uint32_t result = (uint32_t)r8- ((uint32_t)rm8+cf);
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForSub8(result, r8, rm8+cf);
}

CallPtr1632::CallPtr1632(string code_name):Instruction(code_name){

}

void CallPtr1632::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsProtectedMode()){//リアルモード
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t offset;
            uint16_t selector;
            offset = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(4);
            selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
            emu.cpu->AddEip(2);
            InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
            InstructionHelper::Push32(emu, (uint16_t)emu.cpu->GetEip());
            emu.cpu->SetR16(CS, selector);
            emu.cpu->SetEip(offset);
            return;
        }
        uint16_t offset;
        uint16_t selector;
        offset = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(2);
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(2);
        InstructionHelper::Push16(emu, emu.cpu->GetR16(CS));
        InstructionHelper::Push16(emu, (uint16_t)emu.cpu->GetEip());
        emu.cpu->SetR16(CS, selector);
        emu.cpu->SetEip(offset);
        return;
    }
    GdtGate* gdt_gate;
    uint32_t imm32;
    uint16_t selector;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        imm32    = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()+4);
        gdt_gate = emu.cpu->GetGdtGate(selector);
        emu.cpu->AddEip(6);
        if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
            InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
            InstructionHelper::Push32(emu, emu.cpu->GetEip());
            emu.cpu->SetEip(imm32);
            emu.cpu->SetR16(CS, selector);
            return;
        }
        /***
        #define TSS_TYPE 0x09
        #define LDT_TYPE 0x0082
        #define SEGMENT_DESC_TYPE_FLG 1<<4
        ***/
       if((gdt_gate->access_right&CpuEnum::CALL_GATE)!=0){
           uint8_t cpl                    = emu.cpu->GetCpl();
           uint8_t call_gate_dpl          = CpuHelper::GetDpl(gdt_gate->access_right);
           uint8_t call_gate_selector_rpl = CpuHelper::GetRpl(selector);
           uint8_t call_segment_selector  = gdt_gate->base_low;
           uint8_t call_segment_dpl       = CpuHelper::GetDpl(call_segment_selector);
           //GdtGate* call_segment_desc     = emu.cpu->GetGdtGate(call_segment_selector);
           if((call_gate_dpl<cpl)||(call_gate_dpl<call_gate_selector_rpl)){
               this->Error("Not implemented: #GP at %s::Run", this->code_name.c_str());
           }
           if(call_segment_dpl<cpl){// MORE-PRIVILEGE
                uint16_t ss  = emu.cpu->GetR16(SS);
                uint32_t eip = emu.cpu->GetEip();
                uint32_t esp = emu.cpu->GetR32(ESP);
                Tss* tss = emu.cpu->GetCurrentTss();
                emu.cpu->SetR16(SS, tss->ss0);
                emu.cpu->SetR32(ESP, tss->esp0);
                InstructionHelper::Push32(emu, ss);
                InstructionHelper::Push32(emu, esp);
                InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
                InstructionHelper::Push32(emu, eip);
                CallGate* call_gate = (CallGate*)emu.cpu->GetGdtGate(selector);
                uint32_t offset = (((uint32_t)call_gate->offset_high)<<16) | ((uint32_t)call_gate->offset_low);
                emu.cpu->SetEip(offset);
                emu.cpu->SetR16(CS, call_segment_selector);
                return;
           }else{//SAME-PRIVILEGE
                this->Error("Not implemented: SAME-PRIVILEGE at %s::Run", this->code_name.c_str());
           }
           this->Error("Not implemented: CALL_GATE at %s::Run", this->code_name.c_str());
       }
        this->Error("Not implemented: TSS at %s::Run", this->code_name.c_str());
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

Ret32Far::Ret32Far(string code_name):Instruction(code_name){

}

void Ret32Far::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsProtectedMode()){//リアルモードのRET FAR
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t eip;
            uint16_t cs;
            eip     = InstructionHelper::Pop32(emu);
            cs      = InstructionHelper::Pop32(emu);
            emu.cpu->SetEip(eip);
            emu.cpu->SetR16(CS, cs);
            return;
        }
        uint32_t eip;
        uint16_t cs;
        eip     = InstructionHelper::Pop16(emu);
        cs      = InstructionHelper::Pop16(emu);
        emu.cpu->SetEip(eip);
        emu.cpu->SetR16(CS, cs);
        return;
    }
    uint32_t eip;
    uint16_t cs;
    uint32_t esp;
    uint16_t ss;
    uint8_t cpl, rpl;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        eip     = InstructionHelper::Pop32(emu);
        cs      = InstructionHelper::Pop32(emu);
        rpl = CpuHelper::GetRpl(cs);
        cpl = emu.cpu->GetCpl();
        if(rpl>cpl){
            emu.cpu->SetEip(eip);
            emu.cpu->SetR16(CS, cs);
            esp = InstructionHelper::Pop32(emu);
            ss  = InstructionHelper::Pop32(emu);
            emu.cpu->SetR32(ESP, esp);
            emu.cpu->SetR16(SS, ss);
        }else{
            emu.cpu->SetEip(eip);
            emu.cpu->SetR16(CS, cs);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

CallM1632::CallM1632(string code_name):Instruction(code_name){

}

void CallM1632::Run(const Emulator& emu){
    GdtGate* gdt_gate;
    uint16_t selector;
    if(emu.cpu->IsProtectedMode()){
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t effective_addr;
            uint32_t offset_addr;
            effective_addr = this->GetEffectiveAddr(emu);
            offset_addr = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr));
            selector = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4));
            gdt_gate = emu.cpu->GetGdtGate(selector);
            if((gdt_gate->access_right&SEGMENT_DESC_TYPE_FLG)!=0){
                InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
                InstructionHelper::Push32(emu, emu.cpu->GetEip());
                emu.cpu->SetEip(offset_addr);
                emu.cpu->SetR16(CS, selector);
                return;
            }
            this->Error("Not implemented: TSS at %s::Run", this->code_name.c_str());
            return;
        }
        this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    //リアルモードにもcall命令で32bit op sizeはある。
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint16_t effective_addr;
        uint32_t eip;
        effective_addr = this->GetEffectiveAddr(emu);
        eip      = emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr));
        selector = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4));
        InstructionHelper::Push32(emu, emu.cpu->GetR16(CS));
        InstructionHelper::Push32(emu, (uint16_t)emu.cpu->GetEip());
        emu.cpu->SetEip(eip);
        emu.cpu->SetR16(CS, selector);
        return;
    }
    uint16_t effective_addr;
    uint16_t eip;
    effective_addr = this->GetEffectiveAddr(emu);
    eip      = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr));
    selector = emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2));
    InstructionHelper::Push16(emu, emu.cpu->GetR16(CS));
    InstructionHelper::Push16(emu, (uint16_t)emu.cpu->GetEip());
    emu.cpu->SetEip(eip);
    emu.cpu->SetR16(CS, selector);
}

PushSs::PushSs(string code_name):Instruction(code_name){

}

void PushSs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t ss = 0;
        emu.cpu->AddEip(1);
        ss = (uint32_t)emu.cpu->GetR16(SS);
        InstructionHelper::Push32(emu, ss);
        return;
    }
    uint16_t ss;
    emu.cpu->AddEip(1);
    ss = emu.cpu->GetR16(SS);
    InstructionHelper::Push16(emu, ss);
    return;
}

CmpR8Rm8::CmpR8Rm8(string code_name):Instruction(code_name){

}

void CmpR8Rm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t r8;
    uint8_t rm8;
    uint32_t result;
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm8 = this->GetRM8(emu);
    result = (uint32_t)r8 - (uint32_t)rm8;
    emu.cpu->UpdateEflagsForSub8(result, r8, rm8);
    return;
}

MulRm32::MulRm32(string code_name):Instruction(code_name){

}

void MulRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t rm32;
        uint64_t eax;
        uint64_t result;
        eax    = emu.cpu->GetR32(EAX);
        rm32   = this->GetRM32(emu);
        result = eax*rm32;
        emu.cpu->SetR32(EAX, result&0x00000000FFFFFFFF);
        emu.cpu->SetR32(EDX, (result&0xFFFFFFFF00000000)>>32);
        if(!emu.cpu->GetR32(EDX)){
            emu.cpu->ClearFlag(OF);
            emu.cpu->ClearFlag(CF);
        }else{
            emu.cpu->SetFlag(OF);
            emu.cpu->SetFlag(CF);
        }
        return;
    }
    uint32_t rm16;
    uint32_t ax;
    uint32_t result;
    ax    = emu.cpu->GetR16(EAX);
    rm16   = this->GetRM16(emu);
    result = ax*rm16;
    emu.cpu->SetR16(EAX, result&0x0000FFFF);
    emu.cpu->SetR16(EDX, (result&0xFFFF0000)>>16);
    if(!emu.cpu->GetR16(EDX)){
        emu.cpu->ClearFlag(OF);
        emu.cpu->ClearFlag(CF);
    }else{
        emu.cpu->SetFlag(OF);
        emu.cpu->SetFlag(CF);
    }
    return;
}

AddRm8R8::AddRm8R8(string code_name):Instruction(code_name){

}

void AddRm8R8::Run(const Emulator& emu){
    uint8_t r8;
    uint8_t rm8;
    uint16_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8(this->modrm.reg_index); 
    result = (uint16_t)rm8+(uint16_t)r8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm8, r8);
    return;
}

JmpRm32::JmpRm32(string code_name):Instruction(code_name){

}

void JmpRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32 = this->GetRM32(emu);
        emu.cpu->SetEip(rm32);
        return;
    }
    uint32_t rm16 = this->GetRM16(emu);
    emu.cpu->SetEip(rm16);
    return;
}

ShrRm32::ShrRm32(string code_name):Instruction(code_name){

}

void ShrRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t temp_rm32;
        rm32 = this->GetRM32(emu);
        temp_rm32 = rm32;
        if(rm32&1){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(emu, rm32);
        if(temp_rm32&0x80000000){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
        emu.cpu->UpdateEflagsForShr(rm32);
        return;
    }
    uint16_t rm16;
    uint16_t temp_rm16;
    rm16 = this->GetRM16(emu);
    temp_rm16 = rm16;
    if(rm16&1){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm16 = rm16 >> 1;
    this->SetRM16(emu, rm16);
    if(temp_rm16&SIGN_FLG2){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->ClearFlag(OF);
    }
    emu.cpu->UpdateEflagsForShr(rm16);
    return;
}

SalRm32Cl::SalRm32Cl(string code_name):Instruction(code_name){

}

void SalRm32Cl::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t cl;
        bool change_of_flg;
        uint32_t msb_dest;
        rm32 = this->GetRM32(emu);
        cl = emu.cpu->GetR8L(ECX);
        if(cl==0){//回転しないなら、フラグに影響を与えないようにするという仕様
            return;
        }
        change_of_flg = cl==1;
        rm32 = rm32 << (cl-1);
        if(rm32&0x80000000){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 << 1;
        this->SetRM32(emu, rm32);
        if(change_of_flg){
            msb_dest = (0x80000000&rm32)!=0;
            if(msb_dest^emu.cpu->IsFlag(CF)){
                emu.cpu->SetFlag(OF);
            }else{
                emu.cpu->ClearFlag(OF);
            }
        }   
        return;
    }
    uint16_t rm16;
    uint16_t cl;
    bool flg;
    rm16 = this->GetRM16(emu);
    cl = emu.cpu->GetR8L(ECX);
    flg = cl==1;
    if(cl==0){//cl==0の時、何もしない。
        return;
    }
    for(uint16_t i=0; i<cl; i++){
        if(rm16&SIGN_FLG2){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm16 = rm16 << 1;
    }
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);
    if(flg){
        bool msb_dest= (SIGN_FLG2&rm16)?true:false;
        if(msb_dest^emu.cpu->IsFlag(CF)){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
    }   
}

SetgRm8::SetgRm8(string code_name):Instruction(code_name){

}

void SetgRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        if((!emu.cpu->IsFlag(ZF))&&(emu.cpu->IsFlag(CF)==emu.cpu->IsFlag(OF))){
            this->SetRM8(emu, 1);
        }else{
            this->SetRM8(emu, 0);
        }
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SarRm32Cl::SarRm32Cl(string code_name):Instruction(code_name){

}

void SarRm32Cl::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        int32_t rm32;//SAR命令は符号付回転
        uint32_t cl;
        rm32 = this->GetRM32(emu);
        cl = emu.cpu->GetR8L(ECX);
        if(cl==0){//cl==0の時、何もしない。
            return;
        }
        if(cl==1){
            emu.cpu->ClearFlag(OF);
        }
        rm32 = rm32 >> (cl-1);
        if(rm32&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);//shr命令と同じ
        return;
    }
    //TODO:SAR命令は符号を維持して、回転させるので、rm16はint16_tにすべき
    uint16_t rm16;
    uint16_t cl;
    rm16 = this->GetRM16(emu);
    cl = emu.cpu->GetR8L(ECX);
    if(cl==0){//cl==0の時、何もしない。
        return;
    }
    bool flg = (rm16&SIGN_FLG2)? true:false;
    if(cl==1){
        emu.cpu->ClearFlag(OF);
    }
    for(uint32_t i=0; i<cl; i++){
        if(rm16&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm16 = rm16 >> 1;
        rm16 = rm16 | ((flg)?SIGN_FLG2:0);
    }
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);//shr命令と同じ
    return;
}

AndRm32R32::AndRm32R32(string code_name):Instruction(code_name){

}

void AndRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        uint32_t rm32;
        r32 = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(emu);
        result = rm32 & r32;
        this->SetRM32(emu, result);
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t r16;
    uint16_t result;
    uint16_t rm16;
    r16 = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm16 = this->GetRM16(emu);
    result = rm16 & r16;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

SalRm32::SalRm32(string code_name):Instruction(code_name){

}

void SalRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        uint32_t msb_dest;
        rm32 = this->GetRM32(emu);
        if(rm32&0x80000000){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 << 1;
        this->SetRM32(emu, rm32);
        msb_dest = (0x80000000&rm32)!=0;
        if(msb_dest^emu.cpu->IsFlag(CF)){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
        return;
    }
    uint16_t rm16;
    bool msb_dest;
    rm16 = this->GetRM16(emu);
    if(rm16&SIGN_FLG2){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm16 = rm16 << 1;
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);
    msb_dest = (SIGN_FLG2&rm16)?true:false;
    if(msb_dest^emu.cpu->IsFlag(CF)){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->ClearFlag(OF);
    }
    return;
}

AndR32Rm32::AndR32Rm32(string code_name):Instruction(code_name){

}

void AndR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t r32;
        uint32_t result;
        uint32_t rm32;
        r32 = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        rm32 = this->GetRM32(emu);
        result = rm32 & r32;
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
        emu.cpu->UpdateEflagsForAnd(result);
        return;
    }
    uint16_t r16;
    uint16_t result;
    uint16_t rm16;
    r16 = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm16 = this->GetRM16(emu);
    result = rm16 & r16;
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

ShrdRm32R32Imm8::ShrdRm32R32Imm8(string code_name):Instruction(code_name){

}

void ShrdRm32R32Imm8::Run(const Emulator& emu){
    uint8_t imm8;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    if(imm8==8){
        this->Error("Not implemented: imm8==1 at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint64_t rm32, r32;
        uint64_t dest;
        rm32 = this->GetRM32(emu);
        r32  = emu.cpu->GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
        dest = (r32<<32)|rm32;
        dest = dest >> (imm8-1);
        if(dest&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        dest = dest >> 1;
        rm32 = (uint32_t)dest;
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);
        return;
    }
    this->Error("Not implemented: 16bits mode at %s::Run", this->code_name.c_str());
}

SarRm32::SarRm32(string code_name):Instruction(code_name){

}

void SarRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32;
        rm32 = this->GetRM32(emu);
        bool flg = (rm32&SIGN_FLG4)? 1:0;
        emu.cpu->ClearFlag(OF);
        if(rm32&0x01){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm32 = rm32 >> 1;
        rm32 = rm32 | ((flg)?SIGN_FLG4:0);
        this->SetRM32(emu, rm32);
        emu.cpu->UpdateEflagsForShr(rm32);//shr命令と同じ
        return;
    }
    uint16_t rm16;
    rm16 = this->GetRM16(emu);
    bool flg = (rm16&SIGN_FLG2)? true:false;
    emu.cpu->ClearFlag(OF);
    if(rm16&0x01){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm16 = rm16 >> 1;
    rm16 = rm16 | ((flg)?SIGN_FLG2:0);
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);//shr命令と同じ
    return;
}

SetgeRm8::SetgeRm8(string code_name):Instruction(code_name){

}

void SetgeRm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->IsFlag(SF)==emu.cpu->IsFlag(OF)){
        this->SetRM8(emu, 1);
    }else{
        this->SetRM8(emu, 0);
    }
    return;
}

CallRm32::CallRm32(string code_name):Instruction(code_name){

}

void CallRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rm32 = this->GetRM32(emu);
        InstructionHelper::Push32(emu, emu.cpu->GetEip());
        emu.cpu->SetEip(rm32);
        return;
    }
    uint16_t rm16 = this->GetRM16(emu);
    InstructionHelper::Push16(emu, emu.cpu->GetEip());
    emu.cpu->SetEip(rm16);
    return;
}

MovM32M32::MovM32M32(string code_name):Instruction(code_name){

}

void MovM32M32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint32_t ds, es;
    uint32_t esi, edi;
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    ds = emu.cpu->GetBaseAddr(DS);
    es = emu.cpu->GetBaseAddr(ES);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        esi = emu.cpu->GetR32(ESI);
        edi = emu.cpu->GetR32(EDI);
    }else{
        esi = emu.cpu->GetR16(ESI);
        edi = emu.cpu->GetR16(EDI);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.mem->Write(es+edi, emu.mem->Read32(ds+esi));
    }else{
        emu.mem->Write(es+edi, emu.mem->Read16(ds+esi));
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        emu.cpu->SetR32(EDI, edi+d);
        emu.cpu->SetR32(ESI, esi+d);
    }else{
        emu.cpu->SetR16(EDI, edi+d);
        emu.cpu->SetR16(ESI, esi+d);
    }
    return;
}

AdcRm32Imm8::AdcRm32Imm8(string code_name):Instruction(code_name){

}

void AdcRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t imm8;
    uint16_t rm16;
    uint32_t result;
    uint16_t cf = emu.cpu->IsFlag(CF)?1:0;
    rm16 = this->GetRM16(emu);
    imm8 = (int16_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    result = (uint32_t)imm8 + (uint32_t)rm16+(uint32_t)cf;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm16, (uint16_t)(imm8+cf));
    emu.cpu->AddEip(1);
    return;
}

Cwde::Cwde(string code_name):Instruction(code_name){

}

void Cwde::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t al = (int16_t)(int8_t)emu.cpu->GetR8L(EAX);
    emu.cpu->SetR16(EAX, al);
}

AdcRm32R32::AdcRm32R32(string code_name):Instruction(code_name){

}

void AdcRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bits mode at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t rm16   = this->GetRM16(emu);
    uint16_t r16    = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint16_t cf     = emu.cpu->IsFlag(CF)?1:0;
    uint32_t result = (uint32_t)rm16+(uint32_t)r16+(uint32_t)cf;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, rm16, (uint16_t)(r16+cf));
}

LodsM8::LodsM8(string code_name):Instruction(code_name){

}

void LodsM8::Run(const Emulator& emu){
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t ds;
        uint32_t esi;
        uint32_t d = emu.cpu->IsFlag(DF)? -1:1;;
        ds = emu.cpu->GetBaseAddr(DS);
        esi = emu.cpu->GetR32(ESI);
        emu.cpu->SetR8L(EAX, emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+esi)));
        emu.cpu->SetR32(ESI, esi+d);
        return;
    }
    uint32_t ds;
    uint16_t si;
    uint16_t d;
    ds = emu.cpu->GetBaseAddr(DS);
    si = emu.cpu->GetR16(ESI);
    emu.cpu->SetR8L(EAX, emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+si)));
    d = emu.cpu->IsFlag(DF)? -1:1;
    emu.cpu->SetR16(ESI, si+d);
    return;
}

LesR32M1632::LesR32M1632(string code_name):Instruction(code_name){

}

void LesR32M1632::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        uint32_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
        emu.cpu->SetR16(ES, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
        return;
    }
    uint32_t effective_addr;
    effective_addr = this->GetEffectiveAddr(emu);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
    //fprintf(stderr, "ES=%04X\n", emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
    emu.cpu->SetR16(ES, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));

}

MulRm8::MulRm8(string code_name):Instruction(code_name){

}

void MulRm8::Run(const Emulator& emu){
    uint16_t rm8 = this->GetRM8(emu);
    uint16_t al  = emu.cpu->GetR8L(EAX);
    emu.cpu->SetR16(EAX, rm8*al);
    if(!emu.cpu->GetR8H(EAX)){
        emu.cpu->ClearFlag(OF);
        emu.cpu->ClearFlag(CF);
    }else{
        emu.cpu->SetFlag(OF);
        emu.cpu->SetFlag(CF);
    }
}

XchgEaxR32::XchgEaxR32(string code_name):Instruction(code_name){

}

void XchgEaxR32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        this->Error("Not implmented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    GENERAL_PURPOSE_REGISTER32 register_type = (GENERAL_PURPOSE_REGISTER32)((uint32_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess())-(uint32_t)0x90);
    emu.cpu->AddEip(1);
    uint16_t ax = emu.cpu->GetR16(EAX);
    uint16_t r16 = emu.cpu->GetR16(register_type);
    emu.cpu->SetR16(EAX, r16);
    emu.cpu->SetR16(register_type, ax);
}

DivRm8::DivRm8(string code_name):Instruction(code_name){

}

void DivRm8::Run(const Emulator& emu){
    uint16_t ax  = emu.cpu->GetR16(EAX);
    uint16_t rm8 = this->GetRM8(emu);
    emu.cpu->SetR8L(EAX, ax/rm8);
    emu.cpu->SetR8H(EAX, ax%rm8);
}

XchgRm8R8::XchgRm8R8(string code_name):Instruction(code_name){

}

void XchgRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8, r8;
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    this->SetRM8(emu, r8);
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
    return;
}

RorRm8::RorRm8(string code_name):Instruction(code_name){

}

void RorRm8::Run(const Emulator& emu){
    uint8_t rm8;
    rm8 = this->GetRM8(emu);
    uint8_t temp_cf = (rm8&LSB_8)?MSB_8:0;
    rm8 = (rm8>>1) + temp_cf;
    this->SetRM8(emu, rm8);
    if(MSB_8&rm8){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    //結果の最上位2ビットの排他的論理和
    bool bit7, bit6;
    bit7 = (rm8&0x80)?true:false;
    bit6 = (rm8&0x40)?true:false;
    if(bit7^bit6){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->SetFlag(OF);
    }
    return;
}

IncRm8::IncRm8(string code_name):Instruction(code_name){

}

void IncRm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t result;
    uint8_t d = 0x01;
    rm8 = this->GetRM8(emu);
    result = rm8 + d;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForInc8(result, rm8, d);
    return;
}

OrRm8R8::OrRm8R8(string code_name):Instruction(code_name){

}

void OrRm8R8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8;
    uint8_t r8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8|r8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

StosM32::StosM32(string code_name):Instruction(code_name){

}

void StosM32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t edi;
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    uint32_t es = emu.cpu->GetBaseAddr(ES);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        edi = emu.cpu->GetR32(EDI);
    }else{
        edi = emu.cpu->GetR16(EDI);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.cpu->GetR32(EAX));
    }else{
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.cpu->GetR16(EAX));
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        emu.cpu->SetR32(EDI, edi+d);
    }else{
        emu.cpu->SetR16(EDI, edi+d);
    }
    return;
}

LodsM32::LodsM32(string code_name):Instruction(code_name){

}

void LodsM32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t ds = emu.cpu->GetBaseAddr(DS);
    uint32_t esi;
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        esi = emu.cpu->GetR32(ESI);
    }else{
        esi = emu.cpu->GetR16(ESI);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        emu.cpu->SetR32(EAX, emu.mem->Read32(emu.cpu->GetPhysicalAddr(ds+esi)));
    }else{
        emu.cpu->SetR16(EAX, emu.mem->Read16(emu.cpu->GetPhysicalAddr(ds+esi)));
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        emu.cpu->SetR32(ESI, esi+d);
    }else{
        emu.cpu->SetR16(ESI, esi+d);
    }

    return;
}

AndRm8Imm8::AndRm8Imm8(string code_name):Instruction(code_name){

}

void AndRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8, imm8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = rm8&imm8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

AdcR32Rm32::AdcR32Rm32(string code_name):Instruction(code_name){

}

void AdcR32Rm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run");
    }
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint16_t r16;
    uint16_t rm16;
    uint32_t result;
    uint16_t cf = emu.cpu->IsFlag(CF)?1:0;
    r16 = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    rm16 = this->GetRM16(emu);
    result = (uint32_t)r16+ (uint32_t)rm16+(uint32_t)cf;
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAdd(result, r16, (uint16_t)(rm16+cf));
    return;
}

Code66::Code66(string code_name):Instruction(code_name){

}

void Code66::Run(const Emulator& emu){
    emu.cpu->SetPrefix66();
    emu.cpu->AddEip(1);
    return;
}

CodeF3::CodeF3(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0x66] = new Code66("Code66");
    this->instructions[0xA4] = new RepMovsM8M8("RepMovsM8M8");
    this->instructions[0xA5] = new RepMovsM32M32("RepMovsM32M32");
    this->instructions[0xA6] = new RepeCmpsM8M8("RepeCmpsM8M8");
    this->instructions[0xA7] = new RepeCmpsM32M32("RepeCmpsM32M32");
    this->instructions[0xAA] = new RepStosM8("RepStosM8");
    this->instructions[0xAB] = new RepStosM32("RepStosM32");
    this->instructions[0xAE] = new RepeScasM8("RepeScasM8");
    this->instructions[0xAF] = new RepeScasM32("RepeScasM32");
}

CodeF3::~CodeF3(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeF3::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t op_code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    if(op_code==0x66){
        this->instructions[0x66]->Run(emu);
        op_code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    }
    if(this->instructions[op_code]==NULL){
        this->Error("Not implemented: F3 %02X at %s::Run", op_code, this->code_name.c_str());
    }
    this->instructions[op_code]->Run(emu);
    return;
}

RepeCmpsM8M8::RepeCmpsM8M8(string code_name):Instruction(code_name){

}

void RepeCmpsM8M8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment_override at %s::Run", this->code_name.c_str());
    }
    uint32_t base_ds, base_es;
    uint32_t base_ds_esi, base_es_edi;
    uint32_t esi, edi;
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    base_ds = emu.cpu->GetBaseAddr(DS);
    base_es = emu.cpu->GetBaseAddr(ES);
    uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
    for(uint32_t i = 0; i<cnt; i++){
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            esi = emu.cpu->GetR32(ESI);
            edi = emu.cpu->GetR32(EDI);
        }else{
            esi = emu.cpu->GetR16(ESI);
            edi = emu.cpu->GetR16(EDI);
        }
        base_ds_esi = base_ds+esi;
        base_es_edi = base_es+edi;
        uint8_t m1      = emu.mem->Read8(emu.cpu->GetPhysicalAddr(base_ds_esi));
        uint8_t m2      = emu.mem->Read8(emu.cpu->GetPhysicalAddr(base_es_edi));
        uint32_t result = (uint32_t)m1 - (uint32_t)m2;
        emu.cpu->UpdateEflagsForSub8(result, m1, m2);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(ESI, esi+d);
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(ESI, esi+d);
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
        if(!emu.cpu->IsFlag(ZF)){
            return;
        }
    }
    return;
}

CodeF2::CodeF2(string code_name):Instruction(code_name){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SIZE; i++){
        this->instructions[i] = NULL;
    }
    this->instructions[0xAE] = new RepneScasM8("RepneScasM8");
}

CodeF2::~CodeF2(){
    for(int i=0; i<InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++){
        if(this->instructions[i]!=NULL){
            delete this->instructions[i];
        }
    }
}

void CodeF2::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t op_code = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    if(this->instructions[op_code]==NULL){
        this->Error("Not implemented: F2 %02X at %s::Run", op_code, this->code_name.c_str());
    }
    this->instructions[op_code]->Run(emu);
    return;
}

RepneScasM8::RepneScasM8(string code_name):Instruction(code_name){

}

void RepneScasM8::Run(const Emulator& emu){
    //this->Error("Not implemented: %s::Run", this->code_name.c_str());
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit op_size
        if((!emu.cpu->Is32bitsMode()) ^ emu.cpu->IsPrefixAddrSize()){
            this->Error("Not implemented: op_size=32bits && addr_size=16bits at %s::Run", this->code_name.c_str());
        }
        uint32_t cx = emu.cpu->GetR32(ECX);
        uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
        for(uint32_t i = 0; i<cx; i++){
            uint32_t base_es;
            uint32_t edi;
            uint32_t base_es_edi;
            uint8_t al, m8;
            uint32_t result;
            base_es = emu.cpu->GetBaseAddr(ES);
            edi     = emu.cpu->GetR32(EDI);
            base_es_edi = base_es+edi;
            m8      = emu.mem->Read8(base_es_edi);
            al = emu.cpu->GetR8L(EAX);
            result = (uint32_t)al - (uint32_t)m8;
            emu.cpu->UpdateEflagsForSub8(result, al, m8);
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
            if(emu.cpu->IsFlag(ZF)){//等しくなったら終了
                break;
            }
        }
        return;
    }else{//16bit op_size
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            this->Error("Not implemented: addr_size=32bits && addr_size=32bits at %s::Run", this->code_name.c_str());
        }
        uint16_t cx = emu.cpu->GetR16(ECX);
        for(uint16_t i = 0; i<cx; i++){
            uint32_t base_es;
            uint32_t di;
            uint32_t base_es_di;
            uint8_t al, m8;
            uint32_t result;
            uint16_t d;
            base_es = emu.cpu->GetR16(ES)*16;
            di     = emu.cpu->GetR16(EDI);
            base_es_di = base_es+di;
            m8      = emu.mem->Read8(base_es_di);
            al = emu.cpu->GetR8L(EAX);
            result = (uint32_t)al - (uint32_t)m8;
            emu.cpu->UpdateEflagsForSub8(result, al, m8);
            d = emu.cpu->IsFlag(DF)? -1:1;
            emu.cpu->SetR16(EDI, di+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
            if(emu.cpu->IsFlag(ZF)){//等しくなったら終了
                break;
            }
        }
    }
    return;
}

AddAlImm8::AddAlImm8(string code_name):Instruction(code_name){

}

void AddAlImm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t al;
    uint8_t imm8;
    uint16_t result;
    al = emu.cpu->GetR8L(EAX);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = al+imm8;
    emu.cpu->SetR8L(EAX, result);
    emu.cpu->UpdateEflagsForAdd(result, al, imm8);
    return;
}

OrR8Rm8::OrR8Rm8(string code_name):Instruction(code_name){

}

void OrR8Rm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    uint8_t rm8;
    uint8_t r8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8|r8;
    emu.cpu->SetR8(this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

OrR32Rm32::OrR32Rm32(string code_name):Instruction(code_name){

}

void OrR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16;
    uint16_t r16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16 | r16;
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

OrAlImm8::OrAlImm8(string code_name):Instruction(code_name){

}

void OrAlImm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t al, imm8;
    uint8_t result;
    al = emu.cpu->GetR8L(EAX);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = al|imm8;
    emu.cpu->SetR8L(EAX, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

PopSs::PopSs(string code_name):Instruction(code_name){

}

void PopSs::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t ss;
        ss = InstructionHelper::Pop32(emu);
        emu.cpu->SetR16(SS, ss);
        return;
    }
    uint16_t ss;
    ss = InstructionHelper::Pop16(emu);
    emu.cpu->SetR16(SS, ss);
    return;
}

SbbRm32R32::SbbRm32R32(string code_name):Instruction(code_name){

}

void SbbRm32R32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16, r16;
    uint8_t cf = emu.cpu->IsFlag(CF)?1:0;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t result = (uint32_t)rm16- ((uint32_t)r16+(uint32_t)cf);
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForSub16(result, rm16, (uint16_t)r16+(uint16_t)cf);
    return;
}

SbbR32Rm32::SbbR32Rm32(string code_name):Instruction(code_name){

}

void SbbR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16, r16;
    uint8_t cf = emu.cpu->IsFlag(CF)?1:0;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t result = (uint32_t)r16- ((uint32_t)rm16+(uint32_t)cf);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForSub16(result, r16, (uint16_t)rm16+(uint16_t)cf);
    return;
}

SubR8Rm8::SubR8Rm8(string code_name):Instruction(code_name){

}

void SubR8Rm8::Run(const Emulator& emu){
    uint8_t rm8, r8;
    uint8_t cf;
    uint16_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = (uint16_t)r8-(uint16_t)rm8;
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForSub8(result, r8, rm8);
}

SubAlImm8::SubAlImm8(string code_name):Instruction(code_name){

}

void SubAlImm8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t imm8;
    uint8_t al;
    uint32_t result;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    al  = emu.cpu->GetR8L(EAX);
    result = (uint32_t)al - (uint32_t)imm8;
    emu.cpu->SetR8L(EAX, result);
    emu.cpu->UpdateEflagsForSub8(result, al, imm8);
    return;
}

XorRm8R8::XorRm8R8(string code_name):Instruction(code_name){

}

void XorRm8R8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t r8;
    uint8_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8^r8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
    return;
}

XorR8Rm8::XorR8Rm8(string code_name):Instruction(code_name){

}

void XorR8Rm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t r8;
    uint8_t result;
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    rm8 = this->GetRM8(emu);
    r8  = emu.cpu->GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm8^r8;
    emu.cpu->SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
    return;
}

XorR32Rm32::XorR32Rm32(string code_name):Instruction(code_name){

}

void XorR32Rm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
        return;
    }
    uint16_t rm16;
    uint16_t r16;
    uint16_t result;
    rm16 = this->GetRM16(emu);
    r16  = emu.cpu->GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    result = rm16^r16;
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, result);
    emu.cpu->UpdateEflagsForAnd(result);//ANDと更新フラグが同じ
    return;
}

OrRm8Imm8::OrRm8Imm8(string code_name):Instruction(code_name){

}

void OrRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8, imm8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = rm8|imm8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

SubRm8Imm8::SubRm8Imm8(string code_name):Instruction(code_name){

}

void SubRm8Imm8::Run(const Emulator& emu){
    uint8_t imm8;
    uint8_t rm8;
    uint32_t result;
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    rm8  = this->GetRM8(emu);
    result = (uint32_t)rm8 - (uint32_t)imm8;
    emu.cpu->UpdateEflagsForSub8(result, rm8, imm8);
    this->SetRM8(emu, result);
    return;
}

XorRm8Imm8::XorRm8Imm8(string code_name):Instruction(code_name){

}

void XorRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8, imm8;
    uint8_t result;
    rm8 = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = rm8^imm8;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

XorRm32Imm32::XorRm32Imm32(string code_name):Instruction(code_name){

}

void XorRm32Imm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: 32bit=op_size at %s::Run", this->code_name.c_str());
    }
    uint16_t result;
    result = this->GetRM16(emu)^emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForAnd(result);  
    return;
}

SbbRm32Imm8::SbbRm32Imm8(string code_name):Instruction(code_name){

}

void SbbRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16, imm8;
    uint8_t cf = emu.cpu->IsFlag(CF)?1:0;
    rm16 = this->GetRM16(emu);
    imm8 = (int16_t)(int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    uint32_t result = (uint32_t)rm16- ((uint32_t)imm8+(uint32_t)cf);
    this->SetRM16(emu, result);
    emu.cpu->UpdateEflagsForSub16(result, rm16, (uint16_t)imm8+(uint16_t)cf);
    return;
}

PopM32::PopM32(string code_name):Instruction(code_name){

}

void PopM32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t effective_addr;
        effective_addr        = this->GetEffectiveAddr(emu);
        emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(effective_addr), InstructionHelper::Pop32(emu));
        return;
    }
    uint32_t effective_addr;
    effective_addr        = this->GetEffectiveAddr(emu);
    emu.mem->Write(emu.cpu->GetLinearAddrForDataAccess(effective_addr), InstructionHelper::Pop16(emu));
    return;
}

Lahf::Lahf(string code_name):Instruction(code_name){

}

void Lahf::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint8_t eflags = emu.cpu->GetEflgs();
    eflags = eflags | 0x02;
    emu.cpu->SetR8H(EAX, eflags);
}

MovsM8M8::MovsM8M8(string code_name):Instruction(code_name){

}

void MovsM8M8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t ds, es;
        uint32_t esi, edi;
        uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
        ds = emu.cpu->GetBaseAddr(DS);
        esi = emu.cpu->GetR32(ESI);
        es = emu.cpu->GetBaseAddr(ES);
        edi = emu.cpu->GetR32(EDI);
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+esi)));
        emu.cpu->SetR32(EDI, edi+d);
        emu.cpu->SetR32(ESI, esi+d);
        return;
    }
    uint32_t ds, es;
    uint16_t si, di;
    uint16_t d;
    ds = emu.cpu->GetBaseAddr(DS);
    si = emu.cpu->GetR16(ESI);
    es = emu.cpu->GetBaseAddr(ES);
    di = emu.cpu->GetR16(EDI);
    emu.mem->Write(emu.cpu->GetPhysicalAddr(es+di), emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+si)));
    d = emu.cpu->IsFlag(DF)? -1:1;
    emu.cpu->SetR16(EDI, di+d);
    emu.cpu->SetR16(ESI, si+d);
    return;
}

TestAlImm8::TestAlImm8(string code_name):Instruction(code_name){

}

void TestAlImm8::Run(const Emulator& emu){
    uint8_t al;
    uint8_t imm8;
    uint8_t result;
    emu.cpu->AddEip(1);
    al = emu.cpu->GetR8L(EAX);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    result = al & imm8;
    emu.cpu->UpdateEflagsForAnd(result);
    return;
}

StosM8::StosM8(string code_name):Instruction(code_name){

}

void StosM8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t es = emu.cpu->GetBaseAddr(ES);;
        uint32_t edi;
        uint32_t d  = emu.cpu->IsFlag(DF)? -1:1;;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            edi = emu.cpu->GetR32(EDI);
        }else{
            edi = emu.cpu->GetR16(EDI);
        }
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.cpu->GetR8L(EAX));
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(EDI, edi+d);
        }else{
            emu.cpu->SetR16(EDI, edi+d);
        }
    }else{
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            this->Error("Not implemented: addr_size=32bits at %s::Run", this->code_name.c_str());
        }
        uint32_t es;
        uint16_t di;
        uint16_t d;
        es = emu.cpu->GetBaseAddr(ES);
        di = emu.cpu->GetR16(EDI);
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+di), emu.cpu->GetR8L(EAX));
        d = emu.cpu->IsFlag(DF)? -1:1;
        emu.cpu->SetR16(EDI, di+d);
    }
    return;
}

ScasM8::ScasM8(string code_name):Instruction(code_name){

}

void ScasM8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t base_es;
        uint32_t edi;
        uint32_t base_es_edi;
        uint8_t al, m8;
        uint64_t result;
        uint32_t d = emu.cpu->IsFlag(DF)? -1:1;;
        base_es = emu.cpu->GetBaseAddr(ES);
        edi     = emu.cpu->GetR32(EDI);
        base_es_edi = base_es+edi;
        m8      = emu.mem->Read8(emu.cpu->GetPhysicalAddr(base_es_edi));
        al = emu.cpu->GetR8L(EAX);
        result = (uint32_t)al - (uint32_t)m8;
        emu.cpu->UpdateEflagsForSub8(result, al, m8);
        emu.cpu->SetR32(EDI, edi+d);
        return;
    }
    uint32_t base_es;
    uint32_t di;
    uint32_t base_es_di;
    uint8_t al, m8;
    uint32_t result;
    uint16_t d;
    base_es = emu.cpu->GetBaseAddr(ES);
    di     = emu.cpu->GetR16(EDI);
    base_es_di = base_es+di;
    m8      = emu.mem->Read8(emu.cpu->GetPhysicalAddr(base_es_di));
    al = emu.cpu->GetR8L(EAX);
    result = (uint32_t)al - (uint32_t)m8;
    emu.cpu->UpdateEflagsForSub8(result, al, m8);
    d = emu.cpu->IsFlag(DF)? -1:1;
    emu.cpu->SetR16(EDI, di+d);
    return;
}

ScasD::ScasD(string code_name):Instruction(code_name){

}

void ScasD::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t base_es;
    uint32_t edi;
    uint32_t base_es_edi;
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        base_es = emu.cpu->GetBaseAddr(ES);
        edi     = emu.cpu->GetR32(EDI);
        base_es_edi = base_es+edi;
    }else{
        base_es = emu.cpu->GetBaseAddr(ES);
        edi     = emu.cpu->GetR16(EDI);
        base_es_edi = base_es+edi;
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t eax, m32;
        uint64_t result;
        m32      = emu.mem->Read32(base_es_edi);
        eax = emu.cpu->GetR32(EAX);
        result = (uint32_t)eax - (uint32_t)m32;
        emu.cpu->UpdateEflagsForSub(result, eax, m32);
    }else{
        uint16_t ax, m16;
        uint32_t result;
        m16      = emu.mem->Read16(base_es_edi);
        ax = emu.cpu->GetR16(EAX);
        result = (uint32_t)ax - (uint32_t)m16;
        emu.cpu->UpdateEflagsForSub16(result, ax, m16);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        emu.cpu->SetR32(EDI, edi+d);
    }else{
        emu.cpu->SetR16(EDI, edi+d);
    }
    return;
}

//左回転
RclRm32Imm8::RclRm32Imm8(string code_name):Instruction(code_name){

}

void RclRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint8_t imm8 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    uint16_t rm16;
    bool temp_cf;
    bool result_msb;
    rm16    = this->GetRM16(emu);
    for(uint8_t i=0; i<imm8; i++){
        temp_cf = (rm16&MSB_16)?true:false;
        rm16    = (rm16<<1) + (emu.cpu->IsFlag(CF)?1:0);
        if(temp_cf){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
    }
    this->SetRM16(emu , rm16);
    if(imm8==1){
        result_msb = (rm16&MSB_16)?true:false;
        if(result_msb^emu.cpu->IsFlag(CF)){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
    }
}

RetImm16::RetImm16(string code_name):Instruction(code_name){

}

void RetImm16::Run(const Emulator& emu){
    if(emu.cpu->IsProtectedMode()){//下はリアルモード仕様なので、ストップ
        this->Error("Not implemented: protected mode at %s::Run", this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint32_t addr;
    addr = InstructionHelper::Pop16(emu);
    uint16_t imm16 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    emu.cpu->SetEip(addr);
    for(uint16_t i=0; i<imm16; i++){
        InstructionHelper::Pop8(emu);//指定された回数だけPop
    }
    return;
}


LdsR32M1632::LdsR32M1632(string code_name):Instruction(code_name){

}

void LdsR32M1632::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->IsProtectedMode()){
        if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
            uint16_t effective_addr = this->GetEffectiveAddr(emu);
            emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
            emu.cpu->SetR16(DS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
            return;
        }
        this->Error("Not implemented: op_size=16bit_size at %s::Run", this->code_name.c_str());
        return;
    }else{
        if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
            uint16_t effective_addr = this->GetEffectiveAddr(emu);
            emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
            emu.cpu->SetR16(DS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
            return;
        }
        uint16_t effective_addr;
        effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
        //fprintf(stderr, "ES=%04X\n", emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
        emu.cpu->SetR16(DS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
    }
}

RetFarImm16::RetFarImm16(string code_name):Instruction(code_name){
  
}

void RetFarImm16::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsProtectedMode()){
        this->Error("Not implemented: protected mode at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t ip, cs;
    ip = InstructionHelper::Pop16(emu);
    cs = InstructionHelper::Pop16(emu);
    uint16_t imm16 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    emu.cpu->SetEip(ip);
    emu.cpu->SetR16(CS, cs);
    for(uint16_t i=0; i<imm16; i++){
        InstructionHelper::Pop8(emu);//指定された回数だけPop
    }
    return;
}

//左回転
RclRm32::RclRm32(string code_name):Instruction(code_name){

}

void RclRm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16;
    bool temp_cf;
    bool result_msb;
    rm16    = this->GetRM16(emu);
    temp_cf = (rm16&MSB_16)?true:false;
    rm16    = (rm16<<1) + (emu.cpu->IsFlag(CF)?1:0);
    this->SetRM16(emu , rm16);
    if(temp_cf){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    result_msb = (rm16&MSB_16)?true:false;
    if(result_msb^emu.cpu->IsFlag(CF)){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->ClearFlag(OF);
    }
}

SalRm8Cl::SalRm8Cl(string code_name):Instruction(code_name){

}

void SalRm8Cl::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t cl;
    bool flg;
    rm8 = this->GetRM8(emu);
    cl = emu.cpu->GetR8L(ECX);
    flg = cl==1;
    if(cl==0){//cl==0の時、何もしない。
        return;
    }
    for(uint16_t i=0; i<cl; i++){
        if(rm8&SIGN_FLG1){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm8 = rm8 << 1;
    }
    this->SetRM8(emu, rm8);
    emu.cpu->UpdateEflagsForShr(rm8);
    if(flg){
        bool msb_dest= (SIGN_FLG1&rm8)?true:false;
        if(msb_dest^emu.cpu->IsFlag(CF)){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
    }   
}

LoopeRel8::LoopeRel8(string code_name):Instruction(code_name){

}

void LoopeRel8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint32_t count;
    //アドレスサイズによって、カウンタの値が決まる。
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        count = emu.cpu->GetR32(ECX);
        count--;
        emu.cpu->SetR32(ECX, count);
    }else{
        count = emu.cpu->GetR16(ECX);
        count--;
        emu.cpu->SetR16(ECX, count);
    }
    uint32_t rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(count&&emu.cpu->IsFlag(ZF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

LoopRel8::LoopRel8(string code_name):Instruction(code_name){

}

void LoopRel8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint32_t count;
    //アドレスサイズによって、カウンタの値が決まる。
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        count = emu.cpu->GetR32(ECX);
        count--;
        emu.cpu->SetR32(ECX, count);
    }else{
        count = emu.cpu->GetR16(ECX);
        count--;
        emu.cpu->SetR16(ECX, count);
    }
    uint32_t rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(count){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JcxzRel8::JcxzRel8(string code_name):Instruction(code_name){

}

void JcxzRel8::Run(const Emulator& emu){
    //アドレスサイズが32bitの時は、ECXを使用するので、未実装
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        uint32_t rel8;
        emu.cpu->AddEip(1);
        rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
        emu.cpu->AddEip(1);
        if(!emu.cpu->GetR32(ECX)){
            emu.cpu->AddEip(rel8);
        }
        return;
    }
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(!emu.cpu->GetR16(ECX)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

RepMovsM8M8::RepMovsM8M8(string code_name):Instruction(code_name){

}

void RepMovsM8M8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit op_size
        uint32_t cnt;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            cnt = emu.cpu->GetR32(ECX);
        }else{
            cnt = emu.cpu->GetR16(ECX);
        }
        uint32_t ds, es;
        ds = emu.cpu->GetBaseAddr(DS);
        es = emu.cpu->GetBaseAddr(ES);
        uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
        uint16_t si, di;
        uint32_t esi, edi;
        for(uint16_t i = 0; i<cnt; i++){
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                esi = emu.cpu->GetR32(ESI);
                edi = emu.cpu->GetR32(EDI);
            }else{
                si = emu.cpu->GetR16(ESI);
                di = emu.cpu->GetR16(EDI);
            }
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+esi)));
            }else{
                emu.mem->Write(emu.cpu->GetPhysicalAddr(es+di), emu.mem->Read8(emu.cpu->GetPhysicalAddr(ds+si)));
            }
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                emu.cpu->SetR32(EDI, edi+d);
                emu.cpu->SetR32(ESI, esi+d);
                emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
            }else{
                emu.cpu->SetR16(EDI, di+d);
                emu.cpu->SetR16(ESI, si+d);
                emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
            }
        }
        return;
    }else{//16bit op_size
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            this->Error("Not implemented: addr_size=32bits && addr_size=32bits at %s::Run", this->code_name.c_str());
        }
        uint16_t cx = emu.cpu->GetR16(ECX);
        uint32_t ds, es;
        ds = emu.cpu->GetBaseAddr(DS);
        es = emu.cpu->GetBaseAddr(ES);
        for(uint16_t i = 0; i<cx; i++){
            uint16_t si, di;
            uint16_t d;
            si = emu.cpu->GetR16(ESI);
            di = emu.cpu->GetR16(EDI);
            emu.mem->Write(es+di, emu.mem->Read8(ds+si));
            d = emu.cpu->IsFlag(DF)? -1:1;
            emu.cpu->SetR16(EDI, di+d);
            emu.cpu->SetR16(ESI, si+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
    }
    return;
}

RepMovsM32M32::RepMovsM32M32(string code_name):Instruction(code_name){

}

void RepMovsM32M32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    uint32_t ds = emu.cpu->GetBaseAddr(DS);
    uint32_t es = emu.cpu->GetBaseAddr(ES);
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    for(uint32_t i = 0; i<cnt; i++){
        uint32_t esi, edi;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            esi = emu.cpu->GetR32(ESI);
            edi = emu.cpu->GetR32(EDI);
        }else{
            esi = emu.cpu->GetR16(ESI);
            edi = emu.cpu->GetR16(EDI);
        }
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.mem->Read32(emu.cpu->GetPhysicalAddr(ds+esi)));
        }else{
            emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), emu.mem->Read16(emu.cpu->GetPhysicalAddr(ds+esi)));
        }
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ESI, esi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ESI, esi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
    }
    return;
}

RepStosM8::RepStosM8(string code_name):Instruction(code_name){

}

void RepStosM8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    uint8_t  al = emu.cpu->GetR8L(EAX);
    uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
    for(uint16_t i = 0; i<cnt; i++){
        uint32_t es;
        uint32_t edi;
        es  = emu.cpu->GetBaseAddr(ES);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            edi = emu.cpu->GetR32(EDI);
        }else{
            edi = emu.cpu->GetR16(EDI);
        }
        emu.mem->Write(emu.cpu->GetPhysicalAddr(es+edi), al);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
    }
    return;
}


RepStosM32::RepStosM32(string code_name):Instruction(code_name){

}

void RepStosM32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt=emu.cpu->GetR32(ECX);
    }else{
        cnt=emu.cpu->GetR16(ECX);
    }
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit op_size
        uint32_t eax = emu.cpu->GetR32(EAX);
        uint32_t es  = emu.cpu->GetBaseAddr(ES);
        for(uint16_t i = 0; i<cnt; i++){
            uint16_t di;
            uint32_t edi;
            uint32_t d;
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                edi = emu.cpu->GetR32(EDI);
                emu.mem->Write(es+edi, eax);
            }else{
                di = emu.cpu->GetR16(EDI);
                emu.mem->Write(es+di, eax);
            }
            d = emu.cpu->IsFlag(DF)? -4:4;
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                emu.cpu->SetR32(EDI, edi+d);
                emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
            }else{
                emu.cpu->SetR16(EDI, di+d);
                emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
            }
        }
        return;
    }else{//16bit op_size
        uint16_t ax = emu.cpu->GetR16(EAX);
        uint32_t es;
        es = emu.cpu->GetBaseAddr(ES);
        for(uint16_t i = 0; i<cnt; i++){
            uint16_t di;
            uint32_t d;
            uint32_t edi;
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                edi = emu.cpu->GetR32(EDI);
                emu.mem->Write(es+edi, ax);
            }else{
                di = emu.cpu->GetR16(EDI);
                emu.mem->Write(es+di, ax);
            }
            d = emu.cpu->IsFlag(DF)? -2:2;
            if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
                emu.cpu->SetR32(EDI, edi+d);
                emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
            }else{
                emu.cpu->SetR16(EDI, di+d);
                emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
            }
        }
    }
    return;
}

NotRm8::NotRm8(string code_name):Instruction(code_name){

}

void NotRm8::Run(const Emulator& emu){
    uint32_t rm32;
    rm32 = this->GetRM8(emu);
    rm32 = ~rm32;
    this->SetRM8(emu, rm32);
    return;
}

ImulRm16::ImulRm16(string code_name):Instruction(code_name){

}

void ImulRm16::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit_op
        uint64_t rm32;
        uint64_t eax;
        uint64_t result;
        eax    = (int64_t)(int32_t)emu.cpu->GetR32(EAX);
        rm32   = (int64_t)(int32_t)this->GetRM32(emu);
        result = eax*rm32;
        emu.cpu->SetR32(EAX, result&0x00000000FFFFFFFF);
        emu.cpu->SetR32(EDX, (result&0xFFFFFFFF00000000)>>32);
        if(!emu.cpu->GetR32(EDX)){
            emu.cpu->ClearFlag(OF);
            emu.cpu->ClearFlag(CF);
        }else{
            emu.cpu->SetFlag(OF);
            emu.cpu->SetFlag(CF);
        }
        return;
    }
    uint32_t rm16;
    uint32_t ax;
    uint32_t result;
    ax    = (int32_t)(int16_t)emu.cpu->GetR16(EAX);
    rm16   = (int32_t)(int16_t)this->GetRM16(emu);
    result = ax*rm16;
    emu.cpu->SetR16(EAX, result&0x0000FFFF);
    emu.cpu->SetR16(EDX, (result&0xFFFF0000)>>16);
    if(!emu.cpu->GetR16(EDX)){
        emu.cpu->ClearFlag(OF);
        emu.cpu->ClearFlag(CF);
    }else{
        emu.cpu->SetFlag(OF);
        emu.cpu->SetFlag(CF);
    }
    return;
}

Stc::Stc(string code_name):Instruction(code_name){
  
}

void Stc::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.cpu->SetFlag(CF);
}

Std::Std(string code_name):Instruction(code_name){

}

void Std::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    emu.cpu->SetFlag(DF);
    return;
}

ShrRm8::ShrRm8(string code_name):Instruction(code_name){

}

void ShrRm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t temp_rm8;
    rm8  = this->GetRM8(emu);
    temp_rm8 = rm8;
    if(rm8&LSB){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm8 = rm8 >> 1;
    this->SetRM8(emu, rm8);
    emu.cpu->UpdateEflagsForShr(rm8);
    if((temp_rm8&MSB_8)){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->SetFlag(OF);
    }
}

AdcRm8Imm8::AdcRm8Imm8(string code_name):Instruction(code_name){

}

void AdcRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8, imm8;
    uint8_t cf;
    uint16_t result;
    rm8 = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    cf  = emu.cpu->IsFlag(CF)?1:0;
    result = (uint16_t)rm8+(uint16_t)imm8+(uint16_t)cf;
    this->SetRM8(emu, result);
    emu.cpu->UpdateEflagsForAdd((uint16_t)result, (uint8_t)rm8, (uint8_t)(imm8+cf));
    return;
}

XorEaxImm32::XorEaxImm32(string code_name):Instruction(code_name){

}

void XorEaxImm32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: %s::Run", this->code_name.c_str());
    }
    uint16_t result;
    result = emu.cpu->GetR16(EAX) ^ emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(2);
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForAnd(result);//ORとANDのフラグレジスタ更新は同じ
    return;
}

RepeCmpsM32M32::RepeCmpsM32M32(string code_name):Instruction(code_name){

}

void RepeCmpsM32M32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment_override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    for(uint32_t i = 0; i<cnt; i++){
        uint32_t esi, edi;
        uint32_t base_ds = emu.cpu->GetBaseAddr(DS);
        uint32_t base_es = emu.cpu->GetBaseAddr(ES);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            esi = emu.cpu->GetR32(ESI);
            edi = emu.cpu->GetR32(EDI);
        }else{
            esi = emu.cpu->GetR16(ESI);
            edi = emu.cpu->GetR16(EDI);
        }
        uint32_t base_ds_esi = base_ds+esi;
        uint32_t base_es_edi = base_es+edi;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t m1      = emu.mem->Read32(emu.cpu->GetPhysicalAddr(emu.cpu->GetPhysicalAddr(base_ds_esi)));
            uint32_t m2      = emu.mem->Read32(emu.cpu->GetPhysicalAddr(emu.cpu->GetPhysicalAddr(base_es_edi)));
            uint64_t result = (uint64_t)m1 - (uint64_t)m2;
            emu.cpu->UpdateEflagsForSub(result, m1, m2);
        }else{
            uint16_t m1      = emu.mem->Read16(emu.cpu->GetPhysicalAddr(base_ds_esi));
            uint16_t m2      = emu.mem->Read16(emu.cpu->GetPhysicalAddr(base_es_edi));
            uint32_t result = (uint32_t)m1 - (uint32_t)m2;
            emu.cpu->UpdateEflagsForSub16(result, m1, m2);
        }
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(ESI, esi+d);
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(ESI, esi+d);
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
        if(!emu.cpu->IsFlag(ZF)){
            return;
        }
    }
    return;
}

RepeScasM8::RepeScasM8(string code_name):Instruction(code_name){

}

void RepeScasM8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    uint32_t d = emu.cpu->IsFlag(DF)? -1:1;
    uint32_t base_es = emu.cpu->GetBaseAddr(ES);
    for(uint32_t i = 0; i<cnt; i++){
        uint32_t edi;
        uint32_t base_es_edi;
        uint8_t al, m8;
        uint32_t result;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            edi     = emu.cpu->GetR32(EDI);
        }else{
            edi     = emu.cpu->GetR16(EDI);
        }
        base_es_edi = base_es+edi;
        m8      = emu.mem->Read8(emu.cpu->GetPhysicalAddr(base_es_edi));
        al = emu.cpu->GetR8L(EAX);
        result = (uint32_t)al - (uint32_t)m8;
        emu.cpu->UpdateEflagsForSub8(result, al, m8);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
        if(!emu.cpu->IsFlag(ZF)){
            break;
        }
    }
    return;
}

RcrRm32::RcrRm32(string code_name):Instruction(code_name){

}

void RcrRm32::Run(const Emulator& emu){
    bool temp_cf;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit op_size
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint32_t rm16;//16bit目にCFが来るから、uint32_tにしている。
    rm16 = this->GetRM16(emu);
    bool dest_msb = (rm16&MSB_16)?true:false;
    if(dest_msb^emu.cpu->IsFlag(CF)){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->ClearFlag(OF);
    }

    temp_cf = (rm16&LSB)?true:false;
    rm16 = (rm16>>1)+(emu.cpu->IsFlag(CF)?(1<<WORD):0);
    if(temp_cf){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    this->SetRM16(emu, rm16);
    return;
}

AdcEaxImm32::AdcEaxImm32(string code_name):Instruction(code_name){

}

void AdcEaxImm32::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){//32bit op_size
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t imm16;
    uint16_t ax;
    uint32_t result;
    uint16_t cf = emu.cpu->IsFlag(CF)?1:0;
    ax = emu.cpu->GetR16(EAX);
    imm16 = emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess());
    result = (uint32_t)imm16 + (uint32_t)ax+(uint32_t)cf;
    emu.cpu->SetR16(EAX, result);
    emu.cpu->UpdateEflagsForAdd(result, ax, (uint16_t)(imm16+cf));
    emu.cpu->AddEip(2);
    return;
}

RepeScasM32::RepeScasM32(string code_name):Instruction(code_name){

}

void RepeScasM32::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->IsSegmentOverride()){
        this->Error("Not implemented: segment override at %s::Run", this->code_name.c_str());
    }
    uint32_t cnt;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        cnt = emu.cpu->GetR32(ECX);
    }else{
        cnt = emu.cpu->GetR16(ECX);
    }
    uint32_t d;
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        d = emu.cpu->IsFlag(DF)? -4:4;
    }else{
        d = emu.cpu->IsFlag(DF)? -2:2;
    }
    for(uint16_t i = 0; i<cnt; i++){
        uint32_t base_es;
        uint32_t edi;
        uint32_t base_es_edi;
        uint32_t m32;
        uint64_t result;
        base_es = emu.cpu->GetBaseAddr(ES);
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            edi     = emu.cpu->GetR32(EDI);
        }else{
            edi     = emu.cpu->GetR16(EDI);
        }
        base_es_edi = base_es+edi;
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
            uint32_t eax = emu.cpu->GetR32(EAX);
            uint32_t m32      = emu.mem->Read32(emu.cpu->GetPhysicalAddr(base_es_edi));
            uint64_t result = (uint64_t)eax - (uint64_t)m32;
            emu.cpu->UpdateEflagsForSub(result, eax, m32);
        }else{
            uint16_t m16      = emu.mem->Read16(emu.cpu->GetPhysicalAddr(base_es_edi));
            uint16_t ax = emu.cpu->GetR16(EAX);
            uint32_t result = (uint32_t)ax - (uint32_t)m16;
            emu.cpu->UpdateEflagsForSub16(result, ax, m16);
        }
        if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
            emu.cpu->SetR32(EDI, edi+d);
            emu.cpu->SetR32(ECX, emu.cpu->GetR32(ECX)-1);
        }else{
            emu.cpu->SetR16(EDI, edi+d);
            emu.cpu->SetR16(ECX, emu.cpu->GetR16(ECX)-1);
        }
        if(!emu.cpu->IsFlag(ZF)){//等しくなったら終了
            break;
        }
    }
    return;
}

Sahf::Sahf(string code_name):Instruction(code_name){

}

void Sahf::Run(const Emulator& emu){
    uint8_t ah      = emu.cpu->GetR8H(EAX);
    uint32_t eflags = emu.cpu->GetEflgs();
    uint32_t updated_eflags;
    emu.cpu->AddEip(1);
    eflags = eflags & 0xFFFFFF00;
    updated_eflags = eflags|(ah&(SF|ZF|AF|PF|CF));
    emu.cpu->SetEflgs(updated_eflags);
    return;
}

JnpRel8::JnpRel8(string code_name):Instruction(code_name){

}

void JnpRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsFlag(PF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JpRel8::JpRel8(string code_name):Instruction(code_name){

}

void JpRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(PF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

SalRm8::SalRm8(string code_name):Instruction(code_name){

}

void SalRm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        this->Error("Not implemented: op_size=32bit at %s::Run", this->code_name.c_str());
    }
    uint16_t rm16;
    bool msb_dest;
    rm16 = this->GetRM16(emu);
    if(rm16&SIGN_FLG2){
        emu.cpu->SetFlag(CF);
    }else{
        emu.cpu->ClearFlag(CF);
    }
    rm16 = rm16 << 1;
    this->SetRM16(emu, rm16);
    emu.cpu->UpdateEflagsForShr(rm16);
    msb_dest = (SIGN_FLG2&rm16)?true:false;
    if(msb_dest^emu.cpu->IsFlag(CF)){
        emu.cpu->SetFlag(OF);
    }else{
        emu.cpu->ClearFlag(OF);
    }
    return;
}

JnoRel8::JnoRel8(string code_name):Instruction(code_name){

}

void JnoRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(!emu.cpu->IsFlag(OF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JoRel8::JoRel8(string code_name):Instruction(code_name){

}

void JoRel8::Run(const Emulator& emu){
    uint32_t rel8;
    emu.cpu->AddEip(1);
    rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(emu.cpu->IsFlag(OF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

JccRel32::JccRel32(string code_name):Instruction(code_name){

}

void JccRel32::Run(const Emulator& emu){
    uint8_t jcc_type = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    bool condition;
    //TODO : マジックナンバーをenum化する。
    switch(jcc_type){
        case 0x80:
            condition = emu.cpu->IsFlag(OF);
            break;
        case 0x81:
            condition = !emu.cpu->IsFlag(OF);
            break;
        case 0x82:
            condition = emu.cpu->IsFlag(CF);
            break;
        case 0x83:
            condition = !emu.cpu->IsFlag(CF);
            break;
        case 0x84:
            condition = emu.cpu->IsFlag(ZF);
            break;
        case 0x85:
            condition = !emu.cpu->IsFlag(ZF);
            break;
        case 0x86:
            condition = emu.cpu->IsFlag(ZF)||emu.cpu->IsFlag(CF);
            break;
        case 0x87:
            condition = (!emu.cpu->IsFlag(CF))&&(!emu.cpu->IsFlag(ZF));
            break;
        case 0x88:
            condition = emu.cpu->IsFlag(SF);
            break;
        case 0x89:
            condition = !emu.cpu->IsFlag(SF);
            break;
        case 0x8A:
            condition = emu.cpu->IsFlag(PF);
            break;
        case 0x8B:
            condition = !emu.cpu->IsFlag(PF);
            break;
        case 0x8C:
            condition = emu.cpu->IsFlag(SF)!=emu.cpu->IsFlag(OF);
            break;
        case 0x8D:
            condition = emu.cpu->IsFlag(SF)==emu.cpu->IsFlag(OF);
            break;
        case 0x8E:
            condition = emu.cpu->IsFlag(ZF) || (emu.cpu->IsFlag(OF)!=emu.cpu->IsFlag(SF));
            break;
        case 0x8F:
            condition = (!emu.cpu->IsFlag(ZF)) && (emu.cpu->IsFlag(SF)==emu.cpu->IsFlag(OF));
            break;
        default:
            this->Error("Not implemented: jcc_type=0x%02X at %s::Run", jcc_type, this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t rel32;
        rel32 = emu.mem->Read32(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(4);
        if(condition){
            emu.cpu->AddEip(rel32);
        }
        return;
    }
    uint32_t rel16;
    rel16 = (int32_t)((int16_t)emu.mem->Read16(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(2);
    if(condition){
        emu.cpu->AddEip(rel16);
    }
    return;
}

//TODO : LOOP系命令は1つにまとめる
LoopneRel8::LoopneRel8(string code_name):Instruction(code_name){

}

void LoopneRel8::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    uint32_t count;
    //アドレスサイズによって、カウンタの値が決まる。
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixAddrSize()){
        count = emu.cpu->GetR32(ECX);
        count--;
        emu.cpu->SetR32(ECX, count);
    }else{
        count = emu.cpu->GetR16(ECX);
        count--;
        emu.cpu->SetR16(ECX, count);
    }
    uint32_t rel8 = (int32_t)((int8_t)emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess()));
    emu.cpu->AddEip(1);
    if(count&&!emu.cpu->IsFlag(ZF)){
        emu.cpu->AddEip(rel8);
    }
    return;
}

LssR32M1632::LssR32M1632(string code_name):Instruction(code_name){

}

void LssR32M1632::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        uint32_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
        emu.cpu->SetR16(SS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
        return;
    }
    uint32_t effective_addr = this->GetEffectiveAddr(emu);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
    emu.cpu->SetR16(SS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
    return;
}

LfsR32M1632::LfsR32M1632(string code_name):Instruction(code_name){

}

void LfsR32M1632::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        uint16_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
        emu.cpu->SetR16(FS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
        return;
    }
    uint16_t effective_addr = this->GetEffectiveAddr(emu);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
    emu.cpu->SetR16(FS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
    return;
}

LgsR32M1632::LgsR32M1632(string code_name):Instruction(code_name){

}

void LgsR32M1632::Run(const Emulator& emu){
    if(emu.cpu->IsProtectedMode()){
        this->Error("Not implemented: protected mode at %s::Run", this->code_name.c_str());
    }
    emu.cpu->AddEip(1);
    this->ParseModRM(emu);
    if(emu.cpu->Is32bitsMode()^emu.cpu->IsPrefixOpSize()){
        uint16_t effective_addr = this->GetEffectiveAddr(emu);
        emu.cpu->SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read32(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
        emu.cpu->SetR16(GS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+4)));
        return;
    }
    uint16_t effective_addr = this->GetEffectiveAddr(emu);
    emu.cpu->SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr)));
    emu.cpu->SetR16(GS, emu.mem->Read16(emu.cpu->GetLinearAddrForDataAccess(effective_addr+2)));
    return;
}

LldtRm16::LldtRm16(string code_name):Instruction(code_name){

}

void LldtRm16::Run(const Emulator& emu){
    emu.cpu->SetLdtr(this->GetRM16(emu));
    return;
}

PushFs::PushFs(string code_name):Instruction(code_name){

}

void PushFs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t fs;
        emu.cpu->AddEip(1);
        fs = (uint32_t)emu.cpu->GetR16(FS);
        InstructionHelper::Push32(emu, fs);
        return;
    }
    uint16_t fs;
    emu.cpu->AddEip(1);
    fs = emu.cpu->GetR16(FS);
    InstructionHelper::Push16(emu, fs);
    return;
}

PopFs::PopFs(string code_name):Instruction(code_name){

}

void PopFs::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t fs;
        fs = InstructionHelper::Pop32(emu);
        emu.cpu->SetR16(FS, fs);
        return;
    }
    uint16_t fs;
    fs = InstructionHelper::Pop16(emu);
    emu.cpu->SetR16(FS, fs);
    return;
}

PushGs::PushGs(string code_name):Instruction(code_name){

}

void PushGs::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t gs;
        emu.cpu->AddEip(1);
        gs = (uint32_t)emu.cpu->GetR16(GS);
        InstructionHelper::Push32(emu, gs);
        return;
    }
    uint16_t gs;
    emu.cpu->AddEip(1);
    gs = emu.cpu->GetR16(GS);
    InstructionHelper::Push16(emu, gs);
    return;
}

PopGs::PopGs(string code_name):Instruction(code_name){

}

void PopGs::Run(const Emulator& emu){
    emu.cpu->AddEip(1);
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint32_t gs;
        gs = InstructionHelper::Pop32(emu);
        emu.cpu->SetR16(GS, gs);
        return;
    }
    uint16_t gs;
    gs = InstructionHelper::Pop16(emu);
    emu.cpu->SetR16(GS, gs);
    return;
}

SalRm8Imm8::SalRm8Imm8(string code_name):Instruction(code_name){

}

void SalRm8Imm8::Run(const Emulator& emu){
    uint8_t rm8;
    uint8_t imm8;
    rm8  = this->GetRM8(emu);
    imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
    emu.cpu->AddEip(1);
    bool flg = imm8==1;
    if(imm8==0){//cl==0の時、何もしない。
        return;
    }
    for(uint16_t i=0; i<imm8; i++){
        if(rm8&SIGN_FLG1){
            emu.cpu->SetFlag(CF);
        }else{
            emu.cpu->ClearFlag(CF);
        }
        rm8 = rm8 << 1;
    }
    this->SetRM8(emu, rm8);
    emu.cpu->UpdateEflagsForShr(rm8);
    if(flg){
        bool msb_dest= (SIGN_FLG1&rm8)?true:false;
        if(msb_dest^emu.cpu->IsFlag(CF)){
            emu.cpu->SetFlag(OF);
        }else{
            emu.cpu->ClearFlag(OF);
        }
    } 
    return;
}

RolRm32Imm8::RolRm32Imm8(string code_name):Instruction(code_name){

}

void RolRm32Imm8::Run(const Emulator& emu){
    if(emu.cpu->Is32bitsMode() ^ emu.cpu->IsPrefixOpSize()){
        uint8_t imm8 = emu.mem->Read8(emu.cpu->GetLinearAddrForCodeAccess());
        emu.cpu->AddEip(1);
        uint32_t rm32 = this->GetRM32(emu);
        if(imm8==1){
            this->Error("Not implemented: update OF at %s::Run", this->code_name.c_str());
        }
        for(uint8_t i=0; i<imm8; i++){
            if(rm32&SIGN_FLG4){
                emu.cpu->SetFlag(CF);
            }else{
                emu.cpu->ClearFlag(CF);
            }
            rm32 = rm32 << 1;
            rm32 = rm32|(emu.cpu->IsFlag(CF)?1:0);
        }
        this->SetRM32(emu, rm32);
        return;
    }else{
        this->Error("Not implemented: op_size=16bit at %s::Run", this->code_name.c_str());
    }
}