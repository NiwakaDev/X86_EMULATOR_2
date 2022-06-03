#include "../../include/Memory.h"
#include "../../include/Cpu.h"
#include <gtest/gtest.h>
#include <memory>
using namespace std;

TEST(MemoryTest, CheckRead8_OutOfRange_0){
    const int size = 1024;
    //Memory* memory = new Memory(size);
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read8(size), out_of_range);
}

TEST(MemoryTest, CheckRead8_OutOfRange_1){
    const int size = 1024;
    //Memory* memory = new Memory(size);
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read8(size+1), out_of_range);
}

TEST(MemoryTest, CheckRead8_OutOfRange_2){
    const int size = 1024;
    //Memory* memory = new Memory(size);
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read8(1024*1024*1024), out_of_range);
}

TEST(MemoryTest, CheckRead8_OutOfRange_3){
    const int size = 1024;
    //Memory* memory = new Memory(size);
    auto memory = make_unique<Memory>(size);
    EXPECT_NO_THROW(memory->Read8(size-1));
}

TEST(MemoryTest, CheckRead16_OutOfRange_0){
    const int size = 1024;
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read16(size), out_of_range);
}

TEST(MemoryTest, CheckRead16_OutOfRange_1){
    const int size = 1024;
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read16(size+1), out_of_range);
}

TEST(MemoryTest, CheckRead16_OutOfRange_2){
    const int size = 1024;
    auto memory = make_unique<Memory>(size);
    EXPECT_THROW(memory->Read16(1024*1024*1024), out_of_range);
}

TEST(MemoryTest, CheckRead16_OutOfRange_3){
    const int size = 1024;
    auto memory = make_unique<Memory>(size);
    EXPECT_NO_THROW(memory->Read16(size-2));
}


/***
TEST(MemoryTest, CheckSize1){
    const int size = 1024*1024;
    Memory* memory = new Memory(size);
    EXPECT_EQ(memory->GetMemSize(), size);
}

TEST(MemoryTest, CheckSize2){
    const int size = 1024*1024*1024;
    Memory* memory = new Memory(size);
    EXPECT_EQ(memory->GetMemSize(), size);
}

TEST(MemoryTest, CheckOutOfRange0){
    uint8_t data = 0xFA;
    int size     = 1024;
    Memory memory(size);
    memory.Write(size, data);
    EXPECT_EQ(memory.Read8(size), 0);
}

TEST(MemoryTest, CheckOutOfRange1){
    uint8_t data = 0xFA;
    int size     = 1024;
    Memory memory(size);
    memory.Write(size+1, data);
    EXPECT_EQ(memory.Read8(size+1), 0);
}

TEST(MemoryTest, CheckWriteRead1Byte0){
    int size = 1024;
    Memory memory(size);
    const uint8_t data = 0xFF;
    const uint32_t addr = 0;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read8(addr), data);
}

TEST(MemoryTest, CheckWriteRead1Byte1){
    int size = 1024;
    Memory memory(size);
    const uint8_t data = 0xFF;
    const uint32_t addr = 1000;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read8(addr), data);
}

TEST(MemoryTest, CheckWriteRead2Byte0){
    int size = 1024;
    Memory memory(size);
    const uint16_t data = 0xFBFA;
    const uint32_t addr = 1000;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read16(addr), data);
}

TEST(MemoryTest, CheckWriteRead2Byte1){
    int size = 1024;
    Memory memory(size);
    const uint16_t data = 0xFBFA;
    const uint32_t addr = 100;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read16(addr), data);
}

TEST(MemoryTest, CheckWriteRead4Byte0){
    int size = 1024;
    Memory memory(size);
    const uint32_t data = 0xFC12FBFA;
    const uint32_t addr = 100;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read32(addr), data);
}

TEST(MemoryTest, CheckWriteRead4Byte1){
    int size = 1024;
    Memory memory(size);
    const uint32_t data = 0xFC12FBFA;
    const uint32_t addr = 10;
    memory.Write(addr, data);
    EXPECT_EQ(memory.Read32(addr), data);
}
***/