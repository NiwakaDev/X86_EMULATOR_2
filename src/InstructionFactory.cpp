#include "InstructionFactory.h"
#include "Instruction.h"

Instruction* InstructionFactory::CreateInstruction(const uint8_t op_code){
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
    }
    return NULL;
}