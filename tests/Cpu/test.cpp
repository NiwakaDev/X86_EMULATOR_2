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
TEST(CpuTest, CheckSub0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0;
    uint8_t d2 = 0;

    uint8_t result = cpu.Sub(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0); 
}

TEST(CpuTest, CheckSub1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0;
    uint8_t d2 = 1;

    uint8_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0xFF); 
}

TEST(CpuTest, CheckSub2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0;
    uint16_t d2 = 1;

    uint16_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0xFFFF); 
}

TEST(CpuTest, CheckSub3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0;
    uint32_t d2 = 1;

    uint32_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0xFFFFFFFF); 
}

TEST(CpuTest, CheckSub4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x01;

    uint8_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0x7F); 
}

TEST(CpuTest, CheckSub5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x0001;

    uint16_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0x7FFF); 
}

TEST(CpuTest, CheckSub6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x00000001;

    uint32_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0x7FFFFFFF); 
}

TEST(CpuTest, CheckSub7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 2;
    uint8_t d2 = 3;

    uint8_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST(CpuTest, CheckSub8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 2;
    uint16_t d2 = 3;

    uint16_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST(CpuTest, CheckSub9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 2;
    uint32_t d2 = 3;

    uint32_t result = cpu.Sub(d1, d2);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)(d1-d2));
}

//SUB系命令により影響を受けるフラグ
//OF
//SF
//ZF
//AF
//PF
//CF
//ただしAFは実装しない
TEST(CpuTest, CheckAdd0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0;
    uint8_t d2 = 0;
    uint8_t result = cpu.Add(d1, d2);
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdd1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0;
    uint8_t d2 = 1;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 1);
}

TEST(CpuTest, CheckAdd2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x7F;
    uint8_t d2 = 0x01;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckAdd3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x7FFF;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckAdd4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x7FFFFFFF;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}


TEST(CpuTest, CheckAdd5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x01;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdd6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdd7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdd8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xF0;
    uint8_t d2 = 0x01;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xF1);
}

TEST(CpuTest, CheckAdd9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFF0;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFF1);
}

TEST(CpuTest, CheckAdd10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0xFFFFFFF0;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFF1);
}

TEST(CpuTest, CheckAdd11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x80;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x7F);
}

TEST(CpuTest, CheckAdd12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x8000;
    uint16_t result = cpu.Add(d1,d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x7FFF);
}

TEST(CpuTest, CheckAdd13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x80000000;
    uint32_t result = cpu.Add(d1, d2);

    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x7FFFFFFF);
}

TEST(CpuTest, CheckAdd14){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x80;
    uint8_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST(CpuTest, CheckAdd15){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x8000;
    uint16_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdd16){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x80000000;
    uint32_t result = cpu.Add(d1, d2);

    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST(CpuTest, CheckAdc0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xFF;
    uint8_t d2 = 0xFF;
    uint8_t c  = 0x01;

    uint8_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST(CpuTest, CheckAdc1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x80;
    uint8_t c  = 0x01;
    uint8_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)1);
}

TEST(CpuTest, CheckAdc2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0x80;
    uint8_t d2 = 0x00;
    uint8_t c  = 0x01;
    uint8_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x81);
}

TEST(CpuTest, CheckAdc3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x01;
    uint8_t c  = 0x01;
    uint8_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)1);
}

TEST(CpuTest, CheckAdc4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d1 = 0xFF;
    uint8_t d2 = 0xFF;
    uint8_t c  = 0x00;
    uint8_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xFE);
}

TEST(CpuTest, CheckAdc5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0xFFFF;
    uint16_t c  = 0x0001;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST(CpuTest, CheckAdc6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x8000;
    uint16_t c  = 0x0001;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)1);
}

TEST(CpuTest, CheckAdc7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x0000;
    uint16_t c  = 0x0001;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8001);
}

TEST(CpuTest, CheckAdc8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x0001;
    uint16_t c  = 0x0001;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)1);
}

TEST(CpuTest, CheckAdc9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0xFFFF;
    uint16_t c  = 0x0000;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST(CpuTest, CheckAdc10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0xFFFFFFFF;
    uint32_t c  = 0x00000001;
    uint32_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST(CpuTest, CheckAdc11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x80000000;
    uint32_t c  = 0x00000001;
    uint32_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST(CpuTest, CheckAdc12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x00000000;
    uint32_t c  = 0x00000001;
    uint32_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_FALSE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000001);
}

TEST(CpuTest, CheckAdc13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x00000001;
    uint32_t c  = 0x00000001;
    uint32_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)1);
}

TEST(CpuTest, CheckAdc14){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d1 = 0xFFFFFFFF;
    uint16_t d2 = 0xFFFFFFFF;
    uint16_t c  = 0x00000000;
    uint16_t result = cpu.Adc(d1, d2, c);
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(PF)); 
    EXPECT_TRUE(cpu.IsFlag(CF)); 
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFFFFFE);
}
//INC命令により影響を受けるフラグ
//OF
//SF
//ZF
//PF
//AF
//ただしAFは実装しない
TEST(CpuTest, CheckInc0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 1;
    uint32_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)2);
}

TEST(CpuTest, CheckInc1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0xFFFFFFFF;
    uint32_t result = cpu.Inc(d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0);
}

TEST(CpuTest, CheckInc2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0xFFFFFFFE;
    uint32_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST(CpuTest, CheckInc3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d = 0x7FFFFFFF;
    uint32_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckInc4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x0004;
    uint16_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)5);
}

TEST(CpuTest, CheckInc5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x7FFF;
    uint16_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckInc6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0xFFFD;
    uint16_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST(CpuTest, CheckInc7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0xFF;
    uint8_t result = cpu.Inc(d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST(CpuTest, CheckInc8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0xFC;
    uint8_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFD);
}

TEST(CpuTest, CheckInc9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0x7F;
    uint8_t result = cpu.Inc(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

//INC命令により影響を受けるフラグ
//OF
//SF
//ZF
//PF
//AF
//ただしAFは実装しない。CFフラグは影響を受けない。
TEST(CpuTest, CheckDec0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0x00;
    uint8_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST(CpuTest, CheckDec1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x0000;
    uint16_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST(CpuTest, CheckDec2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d      = 0x00000000;
    uint32_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST(CpuTest, CheckDec3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0xFF;
    uint8_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFE);
}

TEST(CpuTest, CheckDec4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0xFFFF;
    uint16_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST(CpuTest, CheckDec5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d      = 0xFFFFFFFF;
    uint32_t result = cpu.Dec(d);
    EXPECT_FALSE(cpu.IsFlag(ZF)); 
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFE);
}

TEST(CpuTest, CheckDec6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t d      = 0x01;
    uint8_t result = cpu.Dec(d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckDec7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t d      = 0x0001;
    uint16_t result = cpu.Dec(d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST(CpuTest, CheckDec8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t d      = 0x00000001;
    uint32_t result = cpu.Dec(d);
    EXPECT_TRUE(cpu.IsFlag(ZF)); 
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

//INC命令により影響を受けるフラグ
//OFはクリア
//CFはクリア
//SF
//ZF
//PF
TEST(CpuTest, CheckAnd0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x00;
    uint8_t data2 = 0x01;

    uint8_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckAnd1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x80;
    uint8_t data2 = 0x00;

    uint8_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckAnd2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x80;
    uint8_t data2 = 0x80;

    uint8_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckAnd3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckAnd4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0001;

    uint16_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckAnd5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckAnd6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x8000;

    uint16_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckAnd7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckAnd8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000001;

    uint32_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckAnd9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckAnd10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x80000000;

    uint32_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckAnd11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu.And(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckOr0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x80;
    uint8_t data2 = 0x00;

    uint8_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckOr1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x80;
    uint8_t data2 = 0x01;

    uint8_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x81);
}

TEST(CpuTest, CheckOr2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckOr3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckOr4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0001;

    uint16_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8001);
}

TEST(CpuTest, CheckOr5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckOr6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckOr7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000001;

    uint32_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000001);
}

TEST(CpuTest, CheckOr8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu.Or(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}