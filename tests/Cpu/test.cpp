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