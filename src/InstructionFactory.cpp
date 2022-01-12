#include "InstructionHelper.h"
#include "Instruction.h"
#include "Instructions.h"

Instruction* InstructionHelper::InstructionFactory::CreateInstruction(const uint8_t op_code){
    switch (op_code){
        case 0x00:
            return new AddRm8R8("AddRm8R8");
        case 0x01:
            return new AddRm32R32("AddRm32R32");
        case 0x02:
            return new AddR8Rm8("AddR8Rm8");
        case 0x03:
            return new AddR32Rm32("AddR32Rm32");
        case 0x04:
            return new AddAlImm8("AddAlImm8");
        case 0x05:
            return new AddEaxImm32("AddEaxImm32");
        case 0x06:
            return new PushEs("PushEs");
        case 0x07:
            return new PopEs("PopEs");
        case 0x08:
            return new OrRm8R8("OrRm8R8");
        case 0x09:
            return new OrRm32R32("OrRm32R32");
        case 0x0A:
            return new OrR8Rm8("OrR8Rm8");
        case 0x0B:
            return new OrR32Rm32("OrR32Rm32");
        case 0x0C:
            return new OrAlImm8("OrAlImm8");
        case 0x0D:
            return new OrEaxImm32("OrEaxImm32");
        case 0x0E:
            return new PushCs("PushCs");
        case 0x0F:
            return new Code0F("Code0F");
        case 0x10:
            return new AdcRm8R8("AdcRm8R8");
        case 0x11:
            return new AdcRm32R32("AdcRm32R32");
        case 0x13:
            return new AdcR32Rm32("AdcR32Rm32");
        case 0x15:
            return new AdcEaxImm32("AdcEaxImm32");
        case 0x16:
            return new PushSs("PushSs");
        case 0x17:
            return new PopSs("PopSs");
        case 0x18:
            return new SbbRm8R8("SbbRm8R8");
        case 0x19:
            return new SbbRm32R32("SbbRm32R32");
        case 0x1A:
            return new SbbR8Rm8("SbbR8Rm8");
        case 0x1B:
            return new SbbR32Rm32("SbbR32Rm32");
        case 0x1E:
            return new PushDs("PushDs");
        case 0x1F:
            return new PopDs("PopDs");
        case 0x20:
            return new AndRm8R8("AndRm8R8");
        case 0x21:
            return new AndRm32R32("AndRm32R32");
        case 0x22:
            return new AndR8Rm8("AndR8Rm8");
        case 0x23:
            return new AndR32Rm32("AndR32Rm32");
        case 0x24:
            return new AndAlImm8("AndAlImm8");
        case 0x25:
            return new AndEaxImm32("AndEaxImm32");
        case 0x28:
            return new SubRm8R8("SubRm8R8");
        case 0x29:
            return new SubRm32R32("SubRm32R32");
        case 0x2A:
            return new SubR8Rm8("SubR8Rm8");
        case 0x2B:
            return new SubR32Rm32("SubR32Rm32");
        case 0x2C:
            return new SubAlImm8("SubAlImm8");
        case 0x2D:
            return new SubEaxImm32("SubEaxImm32");
        case 0x30:
            return new XorRm8R8("XorRm8R8");
        case 0x31:
            return new XorRm32R32("XorRm32R32");
        case 0x32:
            return new XorR8Rm8("XorR8Rm8");
        case 0x33:
            return new XorR32Rm32("XorR32Rm32");
        case 0x35:
            return new XorEaxImm32("XorEaxImm32");
        case 0x38:
            return new CmpRm8R8("CmpRm8R8");
        case 0x39:
            return new CmpRm32R32("CmpRm32R32");
        case 0x3A:
            return new CmpR8Rm8("CmpR8Rm8");
        case 0x3B:
            return new CmpR32Rm32("CmpR32Rm32");
        case 0x3C:
            return new CmpAlImm8("CmpAlImm8");
        case 0x3D:
            return new CmpEaxImm32("CmpEaxImm32");
        case 0x40+0:
        case 0x40+1:
        case 0x40+2:
        case 0x40+3:
        case 0x40+4:
        case 0x40+5:
        case 0x40+6:
        case 0x40+7:
            return new IncR32("IncR32");
        case 0x48+0:
        case 0x48+1:
        case 0x48+2:
        case 0x48+3:
        case 0x48+4:
        case 0x48+5:
        case 0x48+6:
        case 0x48+7:
            return new DecR32("DecR32");
        case 0x50+0:
        case 0x50+1:
        case 0x50+2:
        case 0x50+3:
        case 0x50+4:
        case 0x50+5:
        case 0x50+6:
        case 0x50+7:
            return new PushR32("PushR32");
        case 0x58+0:
        case 0x58+1:
        case 0x58+2:
        case 0x58+3:
        case 0x58+4:
        case 0x58+5:
        case 0x58+6:
        case 0x58+7:
            return new PopR32("PopR32");
        case 0x60:
            return new PushAd("PushAd");
        case 0x61:
            return new PopAd("PopAd");
        case 0x68:
            return new PushImm32("PushImm32");
        case 0x69:
            return new ImulR32Rm32Imm32("ImulR32Rm32Imm32");
        case 0x6A:
            return new PushImm8("PushImm8");
        case 0x6B:
            return new ImulR32Rm32Imm8("ImulR32Rm32Imm8");
        case 0x72:
            return new JcRel8("JcRel8");
        case 0x73:
            return new JaeRel8("JaeRel8");
        case 0x74:
            return new JzRel8("JzRel8");
        case 0x75:
            return new JnzRel8("JnzRel8");
        case 0x76:
            return new JbeRel8("JbeRel8");
        case 0x77:
            return new JaRel8("JaRel8");
        case 0x78:
            return new JsRel8("JsRel8");
        case 0x79:
            return new JnsRel8("JnsRel8");
        case 0x7A:
            return new JpRel8("JpRel8");
        case 0x7B:
            return new JnpRel8("JnpRel8");
        case 0x7C:
            return new JlRel8("JlRel8");
        case 0x7D:
            return new JgeRel8("JgeRel8");
        case 0x7E:
            return new JleRel8("JleRel8");
        case 0x7F:
            return new JgRel8("JgRel8");
        case 0x80:
            return new Code80("Code80");
        case 0x81:
            return new Code81("Code81");
        case 0x83:
            return new Code83("Code83");
        case 0x84:
            return new TestRm8R8("TestRm8R8");
        case 0x85:
            return new TestRm32R32("TestRm32R32");
        case 0x86:
            return new XchgRm8R8("XchgRm8R8");
        case 0x87:
            return new XchgR32Rm32("XchgR32Rm32");
        case 0x88:
            return new MovRm8R8("MovRm8R8");
        case 0x89:
            return new MovRm32R32("MovRm32R32");
        case 0x8A:
            return new MovR8Rm8("MovR8Rm8");
        case 0x8B:
            return new MovR32Rm32("MovR32Rm32");
        case 0x8C:
            return new MovRm16Sreg("MovRm16Sreg");
        case 0x8D:
            return new LeaR32M("LeaR32M");
        case 0x8E:
            return new MovSregRm16("MovSregRm16");
        case 0x8F:
            return new PopM32("PopM32");
        case 0x90:
            return new Nop("Nop");
        case 0x90+1://1から始める理由は0x90にはNopを割り当てているから。
        case 0x90+2:
        case 0x90+3:
        case 0x90+4:
        case 0x90+5:
        case 0x90+6:
        case 0x90+7:
            return new XchgEaxR32("XchgEaxR32");
        case 0x98:
            return new Cwde("Cwde");
        case 0x99:
            return new Cdq("Cdq");
        case 0x9A:
            return new CallPtr1632("CallPtr1632");
        case 0x9C:
            return new PushFd("PushFd");
        case 0x9D:
            return new PopFd("PopFd");
        case 0x9E:
            return new Sahf("Sahf");
        case 0x9F:
            return new Lahf("Lahf");
        case 0xA0:
            return new MovAlMoffs8("MovAlMoffs8");
        case 0xA1:
            return new MovEaxMoffs32("MovEaxMoffs32");
        case 0xA2:
            return new MovMoffs8Al("MovMoffs8Al");
        case 0xA3:
            return new MovMoffs32Eax("MovMoffs32Eax");
        case 0xA4:
            return new MovsM8M8("MovsM8M8");
        case 0xA5:
            return new MovM32M32("MovM32M32");
        case 0xA6:
            return new CmpsM8M8("CmpsM8M8");
        case 0xA8:
            return new TestAlImm8("TestAlImm8");
        case 0xA9:
            return new TestEaxImm32("TestEaxImm32");
        case 0xAA:
            return new StosM8("StosM8");
        case 0xAB:
            return new StosM32("StosM32");
        case 0xAC:
            return new LodsM8("LodsM8");
        case 0xAD:
            return new LodsM32("LodsM32");
        case 0xAE:
            return new ScasM8("ScasM8");
        case 0xAF:
            return new ScasD("ScasD");
        case 0xB0+0:
        case 0xB0+1:
        case 0xB0+2:
        case 0xB0+3:
        case 0xB0+4:
        case 0xB0+5:
        case 0xB0+6:
        case 0xB0+7:
            return new MovR8Imm8("MovR8Imm8");
        case 0xB8+0:
        case 0xB8+1:
        case 0xB8+2:
        case 0xB8+3:
        case 0xB8+4:
        case 0xB8+5:
        case 0xB8+6:
        case 0xB8+7:
            return new MovR32Imm32("MovR32Imm32");
        case 0xC0:
            return new CodeC0("CodeC0");
        case 0xC1:
            return new CodeC1("CodeC1");
        case 0xC2:
            return new RetImm16("RetImm16");
        case 0xC3:
            return new Ret32Near("Ret32Near");
        case 0xC4:
            return new LesR32M1632("LesR32M1632");
        case 0xC5:
            return new LdsR32M1632("LdsR32M1632");
        case 0xC6:
            return new CodeC6("CodeC6");
        case 0xC7:
            return new MovRm32Imm32("MovRm32Imm32");
        case 0xC9:
            return new Leave("Leave");
        case 0xCA:
            return new RetFarImm16("RetFarImm16");
        case 0xCB:
            return new Ret32Far("Ret32Far");
        case 0xCD:
            return new IntImm8("IntImm8");
        case 0xCF:
            return new Iretd("Iretd");
        case 0xD0:
            return new CodeD0("CodeD0");
        case 0xD1:
            return new CodeD1("CodeD1");
        case 0xD2:
            return new CodeD2("CodeD2");
        case 0xD3:
            return new CodeD3("CodeD3");
        case 0xD7:
            return new Xlatb("Xlatb");
        case 0xE1:
            return new LoopeRel8("LoopeRel8");
        case 0xE2:
            return new LoopRel8("LoopRel8");
        case 0xE3:
            return new JcxzRel8("JcxzRel8");
        case 0xE4:
            return new InAlImm8("InAlImm8");
        case 0xE6:
            return new OutImm8("OutImm8");
        case 0xE8:
            return new CallRel32("CallRel32");
        case 0xE9:
            return new JmpRel32("JmpRel32");
        case 0xEA:
            return new JmpPtr1632("JmpPtr1632");
        case 0xEB:
            return new JmpRel8("JmpRel8");
        case 0xEC:
            return new InAlDx("InAlDx");
        case 0xEE:
            return new OutDxAl("OutDxAl");
        case 0xF2:
            return new CodeF2("CodeF2");
        case 0xF3:
            return new CodeF3("CodeF3");
        case 0xF4:
            return new Hlt("Hlt");
        case 0xF6:
            return new CodeF6("CodeF6");
        case 0xF7:
            return new CodeF7("CodeF7");
        case 0xF8:
            return new Clc("Clc");
        case 0xF9:
            return new Stc("Stc");
        case 0xFA:
            return new Cli("Cli");
        case 0xFB:
            return new Sti("Sti");
        case 0xFC:
            return new Cld("Cld");
        case 0xFD:
            return new Std("Std");
        case 0xFE:
            return new CodeFE("CodeFE");
        case 0xFF:
            return new CodeFF("CodeFF");
    }       
    return NULL;
}