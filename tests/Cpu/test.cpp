#include <gtest/gtest.h>
#include <memory>
#include "../../include/Memory.h"
#include "../../include/Cpu.h"

using namespace std;
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

TEST(CpuTest, CheckXor0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckXor1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0x00;
    uint8_t data2 = 0xFF;

    uint8_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST(CpuTest, CheckXor2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t data1 = 0xFF;
    uint8_t data2 = 0xFF;

    uint8_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckXor3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckXor4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0x0000;
    uint16_t data2 = 0xFFFF;

    uint16_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST(CpuTest, CheckXor5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t data1 = 0xFFFF;
    uint16_t data2 = 0xFFFF;

    uint16_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckXor6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckXor7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0xFFFFFFFF;

    uint32_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST(CpuTest, CheckXor8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t data1 = 0xFFFFFFFF;
    uint32_t data2 = 0xFFFFFFFF;

    uint32_t result = cpu.Xor(data1, data2);
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

//SHR命令により影響を受けるフラグ
//SHR命令は右にn bitシフトする。
//このnはオペランドで決まる。
//31回までしか回転しない。
//0回回転の場合は何もしない。
//CF : 右に追い出される最下位bitをここに設定する。
//OF : 1回シフトの場合、元のオペランドの最上位bitをここに設定する。
//SF
//ZF
//PF
TEST(CpuTest, CheckShr0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest=0x00;
    uint8_t temp_count=0x01;
    uint8_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST(CpuTest, CheckShr1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest=0x02;
    uint8_t temp_count=0x01;
    uint8_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x01);
}

TEST(CpuTest, CheckShr2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest=0xFA;
    uint8_t temp_count=0x00;
    uint8_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0xFA);
}

TEST(CpuTest, CheckShr3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x0000;
    uint16_t temp_count = 0x0001;
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST(CpuTest, CheckShr4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest=0x0002;
    uint16_t temp_count=0x0001;
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0001);
}

TEST(CpuTest, CheckShr5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest=0xFBCA;
    uint16_t temp_count=0x00;
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0xFBCA);
}

TEST(CpuTest, CheckShr6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x00000000;
    uint32_t temp_count = 0x00000001;
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckShr7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x00000002;
    uint32_t temp_count = 0x00000001;
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST(CpuTest, CheckShr8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest=0xFBDCCA;
    uint32_t temp_count=0x00;
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFBDCCA);
}

TEST(CpuTest, CheckShr9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest=0xFFFFFFFF;
    uint32_t temp_count=0x20;//32以上はマスクされる。
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST(CpuTest, CheckShr10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x77;
    uint8_t temp_count = 0x02;//32以上はマスクされる。
    uint8_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x1D);
}

TEST(CpuTest, CheckShr11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0xB8F8;
    uint16_t temp_count = 0x0A;//32以上はマスクされる。
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x002E);
}

TEST(CpuTest, CheckShr12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0xB0F8;
    uint16_t temp_count = 0x0A;//32以上はマスクされる。
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x002C);
}

TEST(CpuTest, CheckShr13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0xB61BEC83;
    uint32_t temp_count = 0x12;//32以上はマスクされる。
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00002D86);
}

TEST(CpuTest, CheckShr14){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;//32以上はマスクされる。
    uint16_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x4000);
}

TEST(CpuTest, CheckShr15){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;//32以上はマスクされる。
    uint32_t result = cpu.Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x40000000);
}

//SAL命令とSHL命令は同じ
//SAL命令は左にn bitシフトする。
//このnはオペランドで決まる。
//31回までしか回転しない。
//0回回転の場合は何もしない。
//CF : 左に追い出される最上位bitをここに設定する。
//OF : 1回シフトの場合、最上位2bitが同じなら0にセット、異なるなら1にセット
//SF
//ZF
//PF
TEST(CpuTest, CheckSal0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;//32以上はマスクされる。
    uint8_t result = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckSal1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x02;//32以上はマスクされる。
    uint8_t result = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckSal2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0001;//32以上はマスクされる。
    uint16_t result = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckSal3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0002;//32以上はマスクされる。
    uint16_t result = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST(CpuTest, CheckSal4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000001;//32以上はマスクされる。
    uint32_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x0000);
}

TEST(CpuTest, CheckSal5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000002;//32以上はマスクされる。
    uint32_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_TRUE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckSal6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x00000000;//32以上はマスクされる。
    uint8_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckSal7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0000;//32以上はマスクされる。
    uint16_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckSal8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000000;//32以上はマスクされる。
    uint32_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_FALSE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckSal9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x07;//32以上はマスクされる。
    uint8_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckSal10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x0F;//32以上はマスクされる。
    uint16_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckSal11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x1F;//32以上はマスクされる。
    uint32_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckSal12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0xC0;
    uint8_t temp_count = 0x01;//32以上はマスクされる。
    uint8_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_FALSE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckSal13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0xC000;
    uint16_t temp_count = 0x01;//32以上はマスクされる。
    uint16_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckSal14){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0xC0000000;
    uint32_t temp_count = 0x01;//32以上はマスクされる。
    uint32_t result     = cpu.Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_TRUE(cpu.IsFlag(SF));
    EXPECT_FALSE(cpu.IsFlag(ZF));
    EXPECT_TRUE(cpu.IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

//RCLはCFを最上位bitとして扱う。
//左シフトで追い出された値はCFに格納される。
//影響を受けるフラグ : CF, OF
//※ただし、OFは1bit回転のときに、影響を受ける。
TEST(CpuTest, CheckRcl0){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint8_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckRcl1){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckRcl2){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckRcl3){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;
    cpu.SetFlag(CF);
    uint8_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x01);
}

TEST(CpuTest, CheckRcl4){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;
    cpu.SetFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0001);
}

TEST(CpuTest, CheckRcl5){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;
    cpu.SetFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST(CpuTest, CheckRcl6){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0xC0;
    uint8_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint8_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckRcl7){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0xC000;
    uint16_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckRcl8){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0xC0000000;
    uint32_t temp_count = 0x01;
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST(CpuTest, CheckRcl9){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x40;
    uint8_t temp_count = 0x02;
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint32_t)0x00);
}

TEST(CpuTest, CheckRcl10){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x4000;
    uint16_t temp_count = 0x02;
    cpu.ClearFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckRcl11){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x40000000;
    uint32_t temp_count = 0x02;
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckRcl12){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x21;//(temp_count&0x1F)%9で、1になる。
    cpu.ClearFlag(CF);
    uint8_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST(CpuTest, CheckRcl13){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x29;//(temp_count&0x1F)%9で、0になる。
    cpu.ClearFlag(CF);
    uint8_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST(CpuTest, CheckRcl14){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x21;//(temp_count&0x1F)%17で、1になる。
    cpu.ClearFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST(CpuTest, CheckRcl15){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x31;//(temp_count&0x1F)%17で、0になる。
    cpu.ClearFlag(CF);
    uint16_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST(CpuTest, CheckRcl16){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x21;//temp_count&0x1Fで、1になる。
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu.IsFlag(CF));
    EXPECT_TRUE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST(CpuTest, CheckRcl17){
    auto MockIoIn8    = [&](uint16_t addr){return 0;};
    auto MockIoOut8   = [&](uint16_t addr, uint8_t data){};
    auto MockBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
    int size = 1024*1024;
    Memory memory(size);
    Cpu cpu(MockBiosCall, memory, MockIoIn8, MockIoOut8);

    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x20;//temp_count&0x1Fで、0になる。
    cpu.ClearFlag(CF);
    uint32_t result     = cpu.Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu.IsFlag(CF));
    EXPECT_FALSE(cpu.IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}