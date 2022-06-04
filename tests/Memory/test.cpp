#include "../../include/Memory.h"
#include "../../include/Cpu.h"
#include <gtest/gtest.h>
#include <memory>
using namespace std;

class MemoryTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
            size = 1024;
            memory   = make_unique<Memory>(size);
        }
        unique_ptr<Memory> memory;
        int size;
};

TEST_F(MemoryTest, CheckRead8_OutOfRange_0){
    EXPECT_THROW(memory->Read8(size), out_of_range);
}

TEST_F(MemoryTest, CheckRead8_OutOfRange_1){
    EXPECT_THROW(memory->Read8(size+1), out_of_range);
}

TEST_F(MemoryTest, CheckRead8_OutOfRange_2){
    EXPECT_THROW(memory->Read8(size*size*size), out_of_range);
}

TEST_F(MemoryTest, CheckRead8_OutOfRange_3){
    EXPECT_NO_THROW(memory->Read8(size-1));
}

TEST_F(MemoryTest, CheckRead16_OutOfRange_0){
    EXPECT_THROW(memory->Read16(size), out_of_range);
}

TEST_F(MemoryTest, CheckRead16_OutOfRange_1){
    EXPECT_THROW(memory->Read16(size+1), out_of_range);
}

TEST_F(MemoryTest, CheckRead16_OutOfRange_2){
    EXPECT_THROW(memory->Read16(1024*1024*1024), out_of_range);
}

TEST_F(MemoryTest, CheckRead16_OutOfRange_3){
    EXPECT_NO_THROW(memory->Read16(size-2));
}

TEST_F(MemoryTest, CheckRead16_OutOfRange_4){
    EXPECT_THROW(memory->Read16(size-1), out_of_range);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_0){
    uint32_t addr = size;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), 0);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_1){
    uint32_t addr = size+1;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), 0);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_2){
    uint32_t addr = size-3;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), 0);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_3){
    uint32_t addr = size-2;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), 0);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_4){
    uint32_t addr = size-1;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), 0);
}

TEST_F(MemoryTest, CheckRead32_OutOfRange_5){
    uint32_t addr = size-4;
    uint32_t data = 0xFAFAFAFA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), data);
}

TEST_F(MemoryTest, CheckRead8_0){
    uint32_t addr = 0;
    uint8_t  data = 0xFA; 
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read8(addr), data);
}

TEST_F(MemoryTest, CheckRead8_1){
    uint32_t addr = size-1;
    uint8_t  data = 0x1A;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read8(addr), data);
}

TEST_F(MemoryTest, CheckRead16_0){
    uint32_t addr = 0;
    uint16_t data = 0x1CAA;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read16(addr), data);
}

TEST_F(MemoryTest, CheckRead16_1){
    uint32_t addr = size-2;
    uint16_t data = 0xED23;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read16(addr), data);
}

TEST_F(MemoryTest, CheckRead32_0){
    uint32_t addr = 0;
    uint32_t data = 0x12FFC399;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), data);
}

TEST_F(MemoryTest, CheckRead32_1){
    uint32_t addr = size-4;
    uint32_t data = 0xA2CC4F89;
    memory->Write(addr, data);
    EXPECT_EQ(memory->Read32(addr), data);
}
