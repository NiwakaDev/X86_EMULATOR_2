#include "../include/Memory.h"
#include <gtest/gtest.h>
#include <memory>

/***
//メモ： https://monoist.itmedia.co.jp/mn/articles/1706/28/news017_3.html
//フィクスチャとはテストに必要な準備のことらしい。
class MemoryTestFixture:public::testing::Test{
    protected:
        Memory* memory;
        virtual void SetUp(){
            const int MEM_SIZE = 10;
            memory = new Memory(MEM_SIZE);
        }
        virtual void TearDown(){
            delete memory;
        }
};

TEST_F(MemoryTestFixture, CheckSize){
    EXPECT_EQ(memory->GetMemSize(), 10);
}
***/

TEST(MemoryTest, CheckSize0){
    const int size = 1024;
    Memory* memory = new Memory(size);
    EXPECT_EQ(memory->GetMemSize(), size);
}

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