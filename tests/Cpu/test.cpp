#include <gtest/gtest.h>
#include <memory>
#include "../../include/Memory.h"
#include "../../include/Cpu.h"

using namespace std;

TEST(CpuTest, CheckUpdateZF){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);
    cpu.UpdateZF(0);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    cpu.UpdateZF(1);
    EXPECT_FALSE(cpu.IsFlag(ZF));
}

TEST(CpuTest, CheckUpdateCF){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);
    cpu.UpdateCF(0);
    EXPECT_FALSE(cpu.IsFlag(CF));
    cpu.UpdateCF(0x00000000);
    EXPECT_FALSE(cpu.IsFlag(CF));
    cpu.UpdateCF((uint64_t)0x100000000);
    EXPECT_TRUE(cpu.IsFlag(CF));
}

TEST(CpuTest, CheckUpdatePF0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t result = 0x00000100;
    cpu.UpdatePF(result);
    EXPECT_TRUE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdatePF1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t result = 0x000000FF;
    cpu.UpdatePF(result);
    EXPECT_TRUE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdatePF2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t result = 0x00000006;
    cpu.UpdatePF(result);
    EXPECT_TRUE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdatePF3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t result = 0x00000002;
    cpu.UpdatePF(result);
    EXPECT_FALSE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdatePF4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);


    uint32_t result = 0x00000080;
    cpu.UpdatePF(result);
    EXPECT_FALSE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdatePF5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);


    uint32_t result = 0x00000081;
    cpu.UpdatePF(result);
    EXPECT_TRUE(cpu.IsFlag(PF));
}

TEST(CpuTest, CheckUpdateSF0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t result = 0x80;
    cpu.UpdateSF(result);
    EXPECT_TRUE(result);

    result = 0x00;
    cpu.UpdateSF(result);
    EXPECT_FALSE(result);
}

TEST(CpuTest, CheckUpdateSF1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t result = 0x8000;
    cpu.UpdateSF(result);
    EXPECT_TRUE(result);

    result = 0x0000;
    cpu.UpdateSF(result);
    EXPECT_FALSE(result);
}

TEST(CpuTest, CheckUpdateSF2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t result = 0x80000000;
    cpu.UpdateSF(result);
    EXPECT_TRUE(result);

    result = 0x00000000;
    cpu.UpdateSF(result);
    EXPECT_FALSE(result);
}

//SUB系命令により影響を受けるフラグ
//OF
//SF
//ZF
//AF
//PF
//CF
//ただしAFは実装しない
TEST(CpuTest, CheckUpdateEflagsForSub0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1     = 0;
    uint16_t d2     = 0;
    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1     = 1;
    uint16_t d2     = 0;

    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1     = 0;
    uint16_t d2     = 1;

    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

//CFのテスト
TEST(CpuTest, CheckUpdateEflagsForSub3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1     = 2;
    uint16_t d2     = 3;

    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 2;
    uint32_t d2 = 3;

    uint32_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 2;
    uint8_t d2 = 3;

    uint8_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 2;
    uint8_t d2 = 2;

    uint8_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 2;
    uint16_t d2 = 2;

    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 2;
    uint32_t d2 = 2;

    uint32_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x01;

    uint8_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x0001;

    uint16_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x00000001;

    uint32_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x01;

    uint8_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST(CpuTest, CheckUpdateEflagsForSub13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x01;

    uint8_t result = cpu.UpdateEflagsForSub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

//INC命令により影響を受けるフラグ
//OF
//SF
//ZF
//PF
//AF
//ただしAFは実装しない
TEST(CpuTest, CheckUpdateEflagsForInc0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 1;
    uint32_t result = 1+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0xFFFFFFFF;
    uint32_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0xFFFFFFFE;
    uint32_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0x7FFFFFFF;
    uint32_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x0004;
    uint16_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x7FFF;
    uint16_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0xFFFD;
    uint16_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0xFF;
    uint8_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0xFC;
    uint8_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}

TEST(CpuTest, CheckUpdateEflagsForInc9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0x7F;
    uint8_t result = d+1;
    cpu.UpdateEflagsForInc(result, d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
}