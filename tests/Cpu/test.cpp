#include <gtest/gtest.h>
#include <memory>
#include "../../include/Memory.h"
#include "../../include/Cpu.h"

using namespace std;

class CpuTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
            auto StubIoIn8    = [&](uint16_t addr){return 0;};
            auto StubIoOut8   = [&](uint16_t addr, uint8_t data){};
            auto StubBiosCall = [&](Cpu& cpu, Memory& mem, const uint8_t bios_number){};
            int size = 1024*1024;
            memory = make_unique<Memory>(size);
            cpu    = make_unique<Cpu>(StubBiosCall, *(memory.get()), StubIoIn8, StubIoOut8);
        }
        virtual void TearDown(){

        }
        unique_ptr<Cpu> cpu;
        unique_ptr<Memory> memory;
};
//SUB系命令により影響を受けるフラグ
//OF
//SF
//ZF
//AF
//PF
//CF
//ただしAFは実装しない

TEST_F(CpuTest, CheckSub0){
    uint8_t d1 = 0;
    uint8_t d2 = 0;

    uint8_t result = cpu->Sub(d1, d2);
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0); 
}

TEST_F(CpuTest, CheckSub1){
    uint8_t d1 = 0;
    uint8_t d2 = 1;

    uint8_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0xFF); 
}

TEST_F(CpuTest, CheckSub2){
    uint16_t d1 = 0;
    uint16_t d2 = 1;

    uint16_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0xFFFF); 
}

TEST_F(CpuTest, CheckSub3){
    uint32_t d1 = 0;
    uint32_t d2 = 1;

    uint32_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0xFFFFFFFF); 
}

TEST_F(CpuTest, CheckSub4){
    uint8_t d1 = 0x80;
    uint8_t d2 = 0x01;

    uint8_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0x7F); 
}

TEST_F(CpuTest, CheckSub5){
    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x0001;

    uint16_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0x7FFF); 
}

TEST_F(CpuTest, CheckSub6){
    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x00000001;

    uint32_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0x7FFFFFFF); 
}

TEST_F(CpuTest, CheckSub7){
    uint8_t d1 = 2;
    uint8_t d2 = 3;

    uint8_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)(d1-d2));
}

TEST_F(CpuTest, CheckSub8){
    uint16_t d1 = 2;
    uint16_t d2 = 3;

    uint16_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)(d1-d2));
}

TEST_F(CpuTest, CheckSub9){
    uint32_t d1 = 2;
    uint32_t d2 = 3;

    uint32_t result = cpu->Sub(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
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
TEST_F(CpuTest, CheckAdd0){
    uint8_t d1 = 0;
    uint8_t d2 = 0;
    uint8_t result = cpu->Add(d1, d2);
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdd1){
    uint8_t d1 = 0;
    uint8_t d2 = 1;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 1);
}

TEST_F(CpuTest, CheckAdd2){
    uint8_t d1 = 0x7F;
    uint8_t d2 = 0x01;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckAdd3){
    uint16_t d1 = 0x7FFF;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckAdd4){
    uint32_t d1 = 0x7FFFFFFF;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}


TEST_F(CpuTest, CheckAdd5){
    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x01;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdd6){
    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdd7){
    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdd8){
    uint8_t d1 = 0xF0;
    uint8_t d2 = 0x01;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xF1);
}

TEST_F(CpuTest, CheckAdd9){
    uint16_t d1 = 0xFFF0;
    uint16_t d2 = 0x0001;
    uint16_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFF1);
}

TEST_F(CpuTest, CheckAdd10){
    uint32_t d1 = 0xFFFFFFF0;
    uint32_t d2 = 0x00000001;
    uint32_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFF1);
}

TEST_F(CpuTest, CheckAdd11){
    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x80;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x7F);
}

TEST_F(CpuTest, CheckAdd12){
    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x8000;
    uint16_t result = cpu->Add(d1,d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x7FFF);
}

TEST_F(CpuTest, CheckAdd13){
    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x80000000;
    uint32_t result = cpu->Add(d1, d2);

    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x7FFFFFFF);
}

TEST_F(CpuTest, CheckAdd14){
    uint8_t d1 = 0x80;
    uint8_t d2 = 0x80;
    uint8_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST_F(CpuTest, CheckAdd15){
    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x8000;
    uint16_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdd16){
    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x80000000;
    uint32_t result = cpu->Add(d1, d2);

    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, 0);
}

TEST_F(CpuTest, CheckAdc0){
    uint8_t d1 = 0xFF;
    uint8_t d2 = 0xFF;
    cpu->SetFlag(CF);
    uint8_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST_F(CpuTest, CheckAdc1){
    uint8_t d1 = 0x80;
    uint8_t d2 = 0x80;
    cpu->SetFlag(CF);
    uint8_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)1);
}

TEST_F(CpuTest, CheckAdc2){
    uint8_t d1 = 0x80;
    uint8_t d2 = 0x00;
    cpu->SetFlag(CF);
    uint8_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x81);
}

TEST_F(CpuTest, CheckAdc3){
    uint8_t d1 = 0xFF;
    uint8_t d2 = 0x01;
    cpu->SetFlag(CF);
    uint8_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)1);
}

TEST_F(CpuTest, CheckAdc4){
    uint8_t d1 = 0xFF;
    uint8_t d2 = 0xFF;
    cpu->ClearFlag(CF);
    uint8_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0xFE);
}

TEST_F(CpuTest, CheckAdc5){
    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0xFFFF;
    cpu->SetFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST_F(CpuTest, CheckAdc6){
    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x8000;
    cpu->SetFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)1);
}

TEST_F(CpuTest, CheckAdc7){
    uint16_t d1 = 0x8000;
    uint16_t d2 = 0x0000;
    cpu->SetFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8001);
}

TEST_F(CpuTest, CheckAdc8){
    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0x0001;
    cpu->SetFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)1);
}

TEST_F(CpuTest, CheckAdc9){
    uint16_t d1 = 0xFFFF;
    uint16_t d2 = 0xFFFF;
    cpu->ClearFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST_F(CpuTest, CheckAdc10){
    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0xFFFFFFFF;
    cpu->SetFlag(CF);
    uint32_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST_F(CpuTest, CheckAdc11){
    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x80000000;
    cpu->SetFlag(CF);
    uint32_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST_F(CpuTest, CheckAdc12){
    uint32_t d1 = 0x80000000;
    uint32_t d2 = 0x00000000;
    cpu->SetFlag(CF);
    uint32_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_FALSE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000001);
}

TEST_F(CpuTest, CheckAdc13){
    uint32_t d1 = 0xFFFFFFFF;
    uint32_t d2 = 0x00000001;
    cpu->SetFlag(CF);
    uint32_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)1);
}

TEST_F(CpuTest, CheckAdc14){
    uint16_t d1 = 0xFFFFFFFF;
    uint16_t d2 = 0xFFFFFFFF;
    cpu->ClearFlag(CF);
    uint16_t result = cpu->Adc(d1, d2);
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(PF)); 
    EXPECT_TRUE(cpu->IsFlag(CF)); 
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0xFFFFFFFE);
}
//INC命令により影響を受けるフラグ
//OF
//SF
//ZF
//PF
//AF
//ただしAFは実装しない
TEST_F(CpuTest, CheckInc0){
    uint32_t d = 1;
    uint32_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)2);
}

TEST_F(CpuTest, CheckInc1){
    uint32_t d = 0xFFFFFFFF;
    uint32_t result = cpu->Inc(d);
    EXPECT_TRUE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0);
}

TEST_F(CpuTest, CheckInc2){
    uint32_t d = 0xFFFFFFFE;
    uint32_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST_F(CpuTest, CheckInc3){
    uint32_t d = 0x7FFFFFFF;
    uint32_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckInc4){
    uint16_t d      = 0x0004;
    uint16_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)5);
}

TEST_F(CpuTest, CheckInc5){
    uint16_t d      = 0x7FFF;
    uint16_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckInc6){
    uint16_t d      = 0xFFFD;
    uint16_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST_F(CpuTest, CheckInc7){
    uint8_t d      = 0xFF;
    uint8_t result = cpu->Inc(d);
    EXPECT_TRUE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST_F(CpuTest, CheckInc8){
    uint8_t d      = 0xFC;
    uint8_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFD);
}

TEST_F(CpuTest, CheckInc9){
    uint8_t d      = 0x7F;
    uint8_t result = cpu->Inc(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

//INC命令により影響を受けるフラグ
//OF
//SF
//ZF
//PF
//AF
//ただしAFは実装しない。CFフラグは影響を受けない。
TEST_F(CpuTest, CheckDec0){
    uint8_t d      = 0x00;
    uint8_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST_F(CpuTest, CheckDec1){
    uint16_t d      = 0x0000;
    uint16_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST_F(CpuTest, CheckDec2){
    uint32_t d      = 0x00000000;
    uint32_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST_F(CpuTest, CheckDec3){
    uint8_t d      = 0xFF;
    uint8_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0xFE);
}

TEST_F(CpuTest, CheckDec4){
    uint16_t d      = 0xFFFF;
    uint16_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint16_t)0xFFFE);
}

TEST_F(CpuTest, CheckDec5){
    uint32_t d      = 0xFFFFFFFF;
    uint32_t result = cpu->Dec(d);
    EXPECT_FALSE(cpu->IsFlag(ZF)); 
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFE);
}

TEST_F(CpuTest, CheckDec6){
    uint8_t d      = 0x01;
    uint8_t result = cpu->Dec(d);
    EXPECT_TRUE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckDec7){
    uint16_t d      = 0x0001;
    uint16_t result = cpu->Dec(d);
    EXPECT_TRUE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST_F(CpuTest, CheckDec8){
    uint32_t d      = 0x00000001;
    uint32_t result = cpu->Dec(d);
    EXPECT_TRUE(cpu->IsFlag(ZF)); 
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

//INC命令により影響を受けるフラグ
//OFはクリア
//CFはクリア
//SF
//ZF
//PF
TEST_F(CpuTest, CheckAnd0){
    uint8_t data1 = 0x00;
    uint8_t data2 = 0x01;

    uint8_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckAnd1){
    uint8_t data1 = 0x80;
    uint8_t data2 = 0x00;

    uint8_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckAnd2){
    uint8_t data1 = 0x80;
    uint8_t data2 = 0x80;

    uint8_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckAnd3){
    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckAnd4){
    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0001;

    uint16_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckAnd5){
    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckAnd6){
    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x8000;

    uint16_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckAnd7){
    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckAnd8){
    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000001;

    uint32_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckAnd9){
    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckAnd10){
    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x80000000;

    uint32_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckAnd11){
    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu->And(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckOr0){
    uint8_t data1 = 0x80;
    uint8_t data2 = 0x00;

    uint8_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckOr1){
    uint8_t data1 = 0x80;
    uint8_t data2 = 0x01;

    uint8_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x81);
}

TEST_F(CpuTest, CheckOr2){
    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckOr3){
    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckOr4){
    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x0001;

    uint16_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8001);
}

TEST_F(CpuTest, CheckOr5){
    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckOr6){
    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckOr7){
    uint32_t data1 = 0x80000000;
    uint32_t data2 = 0x00000001;

    uint32_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000001);
}

TEST_F(CpuTest, CheckOr8){
    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu->Or(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckXor0){
    uint8_t data1 = 0x00;
    uint8_t data2 = 0x00;

    uint8_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckXor1){
    uint8_t data1 = 0x00;
    uint8_t data2 = 0xFF;

    uint8_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0xFF);
}

TEST_F(CpuTest, CheckXor2){
    uint8_t data1 = 0xFF;
    uint8_t data2 = 0xFF;

    uint8_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckXor3){
    uint16_t data1 = 0x0000;
    uint16_t data2 = 0x0000;

    uint16_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckXor4){
    uint16_t data1 = 0x0000;
    uint16_t data2 = 0xFFFF;

    uint16_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0xFFFF);
}

TEST_F(CpuTest, CheckXor5){
    uint16_t data1 = 0xFFFF;
    uint16_t data2 = 0xFFFF;

    uint16_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckXor6){
    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0x00000000;

    uint32_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckXor7){
    uint32_t data1 = 0x00000000;
    uint32_t data2 = 0xFFFFFFFF;

    uint32_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST_F(CpuTest, CheckXor8){
    uint32_t data1 = 0xFFFFFFFF;
    uint32_t data2 = 0xFFFFFFFF;

    uint32_t result = cpu->Xor(data1, data2);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
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
TEST_F(CpuTest, CheckShr0){
    uint8_t temp_dest=0x00;
    uint8_t temp_count=0x01;
    uint8_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0);
}

TEST_F(CpuTest, CheckShr1){
    uint8_t temp_dest=0x02;
    uint8_t temp_count=0x01;
    uint8_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x01);
}

TEST_F(CpuTest, CheckShr2){
    uint8_t temp_dest=0xFA;
    uint8_t temp_count=0x00;
    uint8_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0xFA);
}

TEST_F(CpuTest, CheckShr3){
    uint16_t temp_dest  = 0x0000;
    uint16_t temp_count = 0x0001;
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST_F(CpuTest, CheckShr4){
    uint16_t temp_dest=0x0002;
    uint16_t temp_count=0x0001;
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0001);
}

TEST_F(CpuTest, CheckShr5){
    uint16_t temp_dest=0xFBCA;
    uint16_t temp_count=0x00;
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0xFBCA);
}

TEST_F(CpuTest, CheckShr6){
    uint32_t temp_dest  = 0x00000000;
    uint32_t temp_count = 0x00000001;
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckShr7){
    uint32_t temp_dest  = 0x00000002;
    uint32_t temp_count = 0x00000001;
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST_F(CpuTest, CheckShr8){
    uint32_t temp_dest=0xFBDCCA;
    uint32_t temp_count=0x00;
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFBDCCA);
}

TEST_F(CpuTest, CheckShr9){
    uint32_t temp_dest=0xFFFFFFFF;
    uint32_t temp_count=0x20;//32以上はマスクされる。
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
}

TEST_F(CpuTest, CheckShr10){
    uint8_t temp_dest  = 0x77;
    uint8_t temp_count = 0x02;//32以上はマスクされる。
    uint8_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x1D);
}

TEST_F(CpuTest, CheckShr11){
    uint16_t temp_dest  = 0xB8F8;
    uint16_t temp_count = 0x0A;//32以上はマスクされる。
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x002E);
}

TEST_F(CpuTest, CheckShr12){
    uint16_t temp_dest  = 0xB0F8;
    uint16_t temp_count = 0x0A;//32以上はマスクされる。
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x002C);
}

TEST_F(CpuTest, CheckShr13){
    uint32_t temp_dest  = 0xB61BEC83;
    uint32_t temp_count = 0x12;//32以上はマスクされる。
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00002D86);
}

TEST_F(CpuTest, CheckShr14){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;//32以上はマスクされる。
    uint16_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x4000);
}

TEST_F(CpuTest, CheckShr15){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;//32以上はマスクされる。
    uint32_t result = cpu->Shr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
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
TEST_F(CpuTest, CheckSal0){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;//32以上はマスクされる。
    uint8_t result = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckSal1){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x02;//32以上はマスクされる。
    uint8_t result = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckSal2){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0001;//32以上はマスクされる。
    uint16_t result = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckSal3){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0002;//32以上はマスクされる。
    uint16_t result = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x0000);
}

TEST_F(CpuTest, CheckSal4){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000001;//32以上はマスクされる。
    uint32_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x0000);
}

TEST_F(CpuTest, CheckSal5){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000002;//32以上はマスクされる。
    uint32_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckSal6){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x00000000;//32以上はマスクされる。
    uint8_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckSal7){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x0000;//32以上はマスクされる。
    uint16_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckSal8){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x00000000;//32以上はマスクされる。
    uint32_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckSal9){
    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x07;//32以上はマスクされる。
    uint8_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckSal10){
    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x0F;//32以上はマスクされる。
    uint16_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckSal11){
    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x1F;//32以上はマスクされる。
    uint32_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckSal12){
    uint8_t temp_dest  = 0xC0;
    uint8_t temp_count = 0x01;//32以上はマスクされる。
    uint8_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckSal13){
    uint16_t temp_dest  = 0xC000;
    uint16_t temp_count = 0x01;//32以上はマスクされる。
    uint16_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckSal14){
    uint32_t temp_dest  = 0xC0000000;
    uint32_t temp_count = 0x01;//32以上はマスクされる。
    uint32_t result     = cpu->Sal(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

//RCLはCFを最上位bitとして扱う。
//左シフトで追い出された値はCFに格納される。
//影響を受けるフラグ : CF, OF
//※ただし、OFは1bit回転のときに、影響を受ける。
TEST_F(CpuTest, CheckRcl0){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint8_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckRcl1){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckRcl2){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckRcl3){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint8_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x01);
}

TEST_F(CpuTest, CheckRcl4){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0001);
}

TEST_F(CpuTest, CheckRcl5){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000001);
}

TEST_F(CpuTest, CheckRcl6){
    uint8_t temp_dest  = 0xC0;
    uint8_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint8_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRcl7){
    uint16_t temp_dest  = 0xC000;
    uint16_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRcl8){
    uint32_t temp_dest  = 0xC0000000;
    uint32_t temp_count = 0x01;
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckRcl9){
    uint8_t temp_dest  = 0x40;
    uint8_t temp_count = 0x02;
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint32_t)0x00);
}

TEST_F(CpuTest, CheckRcl10){
    uint16_t temp_dest  = 0x4000;
    uint16_t temp_count = 0x02;
    cpu->ClearFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckRcl11){
    uint32_t temp_dest  = 0x40000000;
    uint32_t temp_count = 0x02;
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//2回転なので、falseを期待する。
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckRcl12){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x21;//(temp_count&0x1F)%9で、1になる。
    cpu->ClearFlag(CF);
    uint8_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckRcl13){
    uint8_t temp_dest  = 0x80;
    uint8_t temp_count = 0x29;//(temp_count&0x1F)%9で、0になる。
    cpu->ClearFlag(CF);
    uint8_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRcl14){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x21;//(temp_count&0x1F)%17で、1になる。
    cpu->ClearFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckRcl15){
    uint16_t temp_dest  = 0x8000;
    uint16_t temp_count = 0x31;//(temp_count&0x1F)%17で、0になる。
    cpu->ClearFlag(CF);
    uint16_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRcl16){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x21;//temp_count&0x1Fで、1になる。
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckRcl17){
    uint32_t temp_dest  = 0x80000000;
    uint32_t temp_count = 0x20;//temp_count&0x1Fで、0になる。
    cpu->ClearFlag(CF);
    uint32_t result     = cpu->Rcl(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

//最上位bit以外を右に指定した回数だけシフトする。
//はみ出た値はCFに格納される。
//1回回転の場合は、OFがクリアされる。
TEST_F(CpuTest, CheckSar0){
    uint8_t temp_dest  = 0xF7;
    uint8_t temp_count = 0x02;
    cpu->SetFlag(OF);//2回以上の回転はOFが変化しないはず
    uint8_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint8_t)0xFD);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar1){
    uint8_t temp_dest  = 0xF7;
    uint8_t temp_count = 0x01;
    cpu->SetFlag(OF);
    uint8_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint8_t)0xFB);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar2){
    uint16_t temp_dest  = 0xFFF7;
    uint16_t temp_count = 0x02;
    cpu->SetFlag(OF);//2回以上の回転はOFが変化しないはず
    uint16_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint16_t)0xFFFD);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar3){
    uint16_t temp_dest  = 0xFFF7;
    uint16_t temp_count = 0x01;
    cpu->SetFlag(OF);
    uint16_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint16_t)0xFFFB);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar4){
    uint32_t temp_dest  = 0xFFFFFFF7;
    uint32_t temp_count = 0x02;
    cpu->SetFlag(OF);//2回以上の回転はOFが変化しないはず
    uint32_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFD);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar5){
    uint32_t temp_dest  = 0xFFFFFFF7;
    uint32_t temp_count = 0x01;
    cpu->SetFlag(OF);
    uint32_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFB);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar6){
    uint8_t temp_dest  = 0xF7;
    uint8_t temp_count = 0x20;//1Fでマスクされるので、0回転
    cpu->SetFlag(OF);
    uint8_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint8_t)0xF7);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar7){
    uint16_t temp_dest  = 0xFFF7;
    uint16_t temp_count = 0x20;//1Fでマスクされるので、0回転
    cpu->SetFlag(OF);
    uint16_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint16_t)0xFFF7);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar8){
    uint32_t temp_dest  = 0xFFFFFFF7;
    uint32_t temp_count = 0x20;//1Fでマスクされるので、0回転
    cpu->SetFlag(OF);
    uint32_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint32_t)0xFFFFFFF7);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar9){
    uint8_t temp_dest  = 0xF7;
    uint8_t temp_count = 0x04;
    cpu->SetFlag(OF);
    uint8_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint8_t)0xFF);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar10){
    uint16_t temp_dest  = 0xFFF7;
    uint16_t temp_count = 0x04;
    cpu->SetFlag(OF);
    uint16_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint16_t)0xFFFF);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar11){
    uint32_t temp_dest  = 0xFFFFFFF7;
    uint32_t temp_count = 0x04;
    cpu->SetFlag(OF);
    uint32_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint32_t)0xFFFFFFFF);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar12){
    uint8_t temp_dest  = 0x7F;
    uint8_t temp_count = 0x02;
    cpu->SetFlag(OF);
    uint8_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint8_t)0x1F);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar13){
    uint16_t temp_dest  = 0x7FFF;
    uint16_t temp_count = 0x02;
    cpu->SetFlag(OF);
    uint16_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint16_t)0x1FFF);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

TEST_F(CpuTest, CheckSar14){
    uint32_t temp_dest  = 0x7FFFFFFF;
    uint32_t temp_count = 0x02;
    cpu->SetFlag(OF);
    uint32_t result     = cpu->Sar(temp_dest, temp_count);
    EXPECT_EQ(result, (uint32_t)0x1FFFFFFF);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(CF));
}

//ROR命令はRCR命令と異なりCFを1つのbitとして扱わない。
//SF、ZF、AF、PFは影響を受けない。
//CFは右に追い出された値が格納される。これは最上位bitと等しくなる。
//OFは１回転のみ影響を受ける。最上位2bitの排他的論理和がOFに格納される。
TEST_F(CpuTest, CheckRor0){
    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x01;
    cpu->ClearFlag(OF);//1回転なので、OFは影響を受ける。
    uint8_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRor1){
    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x0001;
    cpu->ClearFlag(OF);//1回転なので、OFは影響を受ける。
    uint16_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRor2){
    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x00000001;
    cpu->ClearFlag(OF);//1回転なので、OFは影響を受ける。
    uint32_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckRor3){
    uint8_t temp_dest  = 0x02;
    uint8_t temp_count = 0x02;
    cpu->ClearFlag(OF);//2回転なので、OFは影響を受けないはず
    uint8_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRor4){
    uint16_t temp_dest  = 0x0002;
    uint16_t temp_count = 0x02;
    cpu->ClearFlag(OF);//2回転なので、OFは影響を受けないはず
    uint16_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRor5){
    uint8_t temp_dest  = 0x88;
    uint8_t temp_count = 0x03;
    cpu->ClearFlag(OF);
    uint8_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x11);
}

TEST_F(CpuTest, CheckRor6){
    uint16_t temp_dest  = 0x8008;
    uint16_t temp_count = 0x03;
    cpu->ClearFlag(OF);
    uint16_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x1001);
}

TEST_F(CpuTest, CheckRor7){
    uint32_t temp_dest  = 0x80000008;
    uint32_t temp_count = 0x03;
    cpu->ClearFlag(OF);
    uint32_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x10000001);
}

TEST_F(CpuTest, CheckRor8){
    uint8_t temp_dest  = 0x02;
    uint8_t temp_count = 0x01;
    cpu->ClearFlag(OF);
    uint8_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//1回転だが、OFはFALSEになるはず
    EXPECT_EQ(result, (uint8_t)0x01);
}

TEST_F(CpuTest, CheckRor9){
    uint16_t temp_dest  = 0x0002;
    uint16_t temp_count = 0x01;
    cpu->ClearFlag(OF);
    uint16_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//1回転だが、OFはFALSEになるはず
    EXPECT_EQ(result, (uint16_t)0x01);
}

TEST_F(CpuTest, CheckRor10){
    uint32_t temp_dest  = 0x00000002;
    uint32_t temp_count = 0x01;
    cpu->ClearFlag(OF);
    uint32_t result     = cpu->Ror(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));//1回転だが、OFはFALSEになるはず
    EXPECT_EQ(result, (uint32_t)0x01);
}

TEST_F(CpuTest, CheckRcr0){
    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x01;
    uint8_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x00);
}

TEST_F(CpuTest, CheckRcr1){
    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x01;
    uint16_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x0000);
}

TEST_F(CpuTest, CheckRcr2){
    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x01;
    uint32_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x00000000);
}

TEST_F(CpuTest, CheckRcr3){
    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint8_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRcr4){
    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint16_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRcr5){
    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x01;
    cpu->SetFlag(CF);
    uint32_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

TEST_F(CpuTest, CheckRcr6){
    uint8_t temp_dest  = 0x01;
    uint8_t temp_count = 0x02;
    uint8_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint8_t)0x80);
}

TEST_F(CpuTest, CheckRcr7){
    uint16_t temp_dest  = 0x0001;
    uint16_t temp_count = 0x02;
    uint16_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint16_t)0x8000);
}

TEST_F(CpuTest, CheckRcr8){
    uint32_t temp_dest  = 0x00000001;
    uint32_t temp_count = 0x02;
    uint32_t result     = cpu->Rcr(temp_dest, temp_count);
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_EQ(result, (uint32_t)0x80000000);
}

//STOS命令で影響を受けるフラグ : 無し
//DF = 1 : EDI or DIがデクリメントされる
//DF = 0 : EDI or DIがインクリメントされる
//インクリメントもしくはデクリメントされる数値は、転送されるデータの大きさで決まる。
//al  : 1 インクリメントorデクリメント
//ax  : 2 インクリメントorデクリメント
//eax : 4 インクリメントorデクリメント
//al, ax, eaxをデスティネーションオペランドに格納する。
//ディスティネーションオペランドは、ES:EDI or ES:DIが示す番地
//ESはセグメントオーバーライドすることはできない。
//EDI : アドレスサイズが32bitの時に利用される。
//DI  : アドレスサイズが16bitの時に利用される。

//Stos命令の全てをreal modeで実行する。
TEST_F(CpuTest, CheckStos0){
    uint8_t al = 0xFA;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 0);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(al);
    EXPECT_EQ(memory->Read8(addr), al);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)0x00000001);
}

TEST_F(CpuTest, CheckStos1){
    uint16_t ax = 0x12FA;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 0);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(ax);
    EXPECT_EQ(memory->Read16(addr), ax);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)0x00000002);
}

TEST_F(CpuTest, CheckStos2){
    uint32_t eax = 0x7DBC12FA;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 0);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(eax);
    EXPECT_EQ(memory->Read32(addr), eax);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)0x00000004);
}

TEST_F(CpuTest, CheckStos3){
    uint8_t al = 0xFA;
    cpu->SetFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 100);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(al);
    EXPECT_EQ(memory->Read8(addr), al);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)99);
}

TEST_F(CpuTest, CheckStos4){
    uint16_t ax = 0x12FA;
    cpu->SetFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 100);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(ax);
    EXPECT_EQ(memory->Read16(addr), ax);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)98);
}

TEST_F(CpuTest, CheckStos5){
    uint32_t eax = 0x7DBC12FA;
    cpu->SetFlag(DF);
    cpu->On32bitMode();//アドレスサイズが32bitになる。
    cpu->SetR32(EDI, 100);
    uint32_t addr = cpu->GetBaseAddr(ES)+cpu->GetR32(EDI);
    cpu->Stos(eax);
    EXPECT_EQ(memory->Read32(addr), eax);
    EXPECT_EQ(cpu->GetR32(EDI), (uint32_t)96);
}

//Scas命令で影響を受けるフラグ : OF, SF, ZF, AF, PF, CF
//ただし、AFの更新処理はX86_EMULATOR2では行わない。
//メモリオペランドで指定されたバイト、ワード、ダブルワードを
//AL, AX, EAXの値と比較する。
//アドレスサイズによって、EDI or DIのどちらを使用するかが決まる。
//比較した後、EDI or DIは、DFによって、インクリメント or デクリメントが行われる。
//DF = 0 : インクリメント
//DF = 1 : デクリメント
//ESはセグメントオーバーライドすることはできない。
TEST_F(CpuTest, CheckScas0){
    uint8_t al = 0x00;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), al);
    cpu->Scas(al);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
}

TEST_F(CpuTest, CheckScas1){
    uint16_t ax = 0x0000;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), ax);
    cpu->Scas(ax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
}

TEST_F(CpuTest, CheckScas2){
    uint32_t eax = 0x00000000;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), eax);
    cpu->Scas(eax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
}

TEST_F(CpuTest, CheckScas3){
    uint8_t al = 0;
    EXPECT_NO_THROW(cpu->Scas(al));

    uint16_t ax = 0;
    EXPECT_NO_THROW(cpu->Scas(ax));

    uint32_t eax = 0;
    EXPECT_NO_THROW(cpu->Scas(eax));

    uint64_t rax = 0x00000000;
    EXPECT_THROW(cpu->Scas(rax), runtime_error);
}

TEST_F(CpuTest, CheckScas4){
    uint8_t al = 0x00;
    uint8_t memory_operand = 0x01;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(al);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
}

TEST_F(CpuTest, CheckScas5){
    uint16_t ax = 0x0000;
    uint16_t memory_operand = 0x0001;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(ax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
}

TEST_F(CpuTest, CheckScas6){
    uint32_t eax = 0x00000000;
    uint32_t memory_operand = 0x00000001;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(eax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
}

TEST_F(CpuTest, CheckScas7){
    uint8_t al = 0x80;
    uint8_t memory_operand = 0x01;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(al);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
}

TEST_F(CpuTest, CheckScas8){
    uint16_t ax = 0x8000;
    uint16_t memory_operand = 0x0001;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(ax);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
}

TEST_F(CpuTest, CheckScas9){
    uint32_t eax = 0x80000000;
    uint32_t memory_operand = 0x00000001;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(eax);
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
}

TEST_F(CpuTest, CheckScas10){
    uint8_t al = 0x2;
    uint8_t memory_operand = 0x3;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(al);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
}

TEST_F(CpuTest, CheckScas11){
    uint16_t ax = 0x2;
    uint16_t memory_operand = 0x3;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(ax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
}

TEST_F(CpuTest, CheckScas12){
    uint32_t eax = 0x2;
    uint32_t memory_operand = 0x3;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), memory_operand);
    cpu->Scas(eax);
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
}

//Cmps命令で影響を受けるフラグ : OF, SF, ZF, AF, PF, CF
//ただし、AFの更新処理はX86_EMULATOR2では行わない。
//メモリオペランドで指定されたバイト、ワード、ダブルワード同士を比較する
//アドレスサイズによって、「EDI or DI」のどちらを使用するかが決まる。
//「ESI, SI」も同様にアドレスサイズで決まる。
//比較した後、「EDI or DI」と「ESI or SI」は、DFによって、インクリメント or デクリメントが行われる。
//DF = 0 : インクリメント
//DF = 1 : デクリメント
//ESはセグメントオーバーライドすることはできない。
//DSはオーバーライドできる。
TEST_F(CpuTest, CheckCmps0){
    uint8_t m1 = 0;
    uint8_t m2 = 0;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
    EXPECT_EQ(cpu->GetR32(ESI), 1001);
}

TEST_F(CpuTest, CheckCmps1){
    uint16_t m1 = 0;
    uint16_t m2 = 0;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
    EXPECT_EQ(cpu->GetR32(ESI), 1002);
}

TEST_F(CpuTest, CheckCmps2){
    uint32_t m1 = 0;
    uint32_t m2 = 0;
    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_TRUE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
    EXPECT_EQ(cpu->GetR32(ESI), 1004);
}

TEST_F(CpuTest, CheckCmps3){
    uint64_t m1 = 0;
    EXPECT_THROW(cpu->Cmps(sizeof(m1)), runtime_error);
}

TEST_F(CpuTest, CheckCmps4){
    uint8_t m1 = 0;
    uint8_t m2 = 1;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
    EXPECT_EQ(cpu->GetR32(ESI), 1001);
}

TEST_F(CpuTest, CheckCmps5){
    uint16_t m1 = 0;
    uint16_t m2 = 1;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
    EXPECT_EQ(cpu->GetR32(ESI), 1002);
}

TEST_F(CpuTest, CheckCmps6){
    uint32_t m1 = 0;
    uint32_t m2 = 1;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
    EXPECT_EQ(cpu->GetR32(ESI), 1004);
}

TEST_F(CpuTest, CheckCmps7){
    uint8_t m1 = 0x80;
    uint8_t m2 = 0x01;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_FALSE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
    EXPECT_EQ(cpu->GetR32(ESI), 1001);
}

TEST_F(CpuTest, CheckCmps8){
    uint16_t m1 = 0x8000;
    uint16_t m2 = 0x0001;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
    EXPECT_EQ(cpu->GetR32(ESI), 1002);
}

TEST_F(CpuTest, CheckCmps9){
    uint32_t m1 = 0x80000000;
    uint32_t m2 = 0x0001;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_TRUE(cpu->IsFlag(OF));
    EXPECT_FALSE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_FALSE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
    EXPECT_EQ(cpu->GetR32(ESI), 1004);
}

TEST_F(CpuTest, CheckCmps10){
    uint8_t m1 = 2;
    uint8_t m2 = 3;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 1);
    EXPECT_EQ(cpu->GetR32(ESI), 1001);
}

TEST_F(CpuTest, CheckCmps11){
    uint16_t m1 = 2;
    uint16_t m2 = 3;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 2);
    EXPECT_EQ(cpu->GetR32(ESI), 1002);
}

TEST_F(CpuTest, CheckCmps12){
    uint32_t m1 = 2;
    uint32_t m2 = 3;

    cpu->ClearFlag(DF);
    cpu->On32bitMode();
    cpu->SetR32(EDI, 0);
    cpu->SetR32(ESI, 1000);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+cpu->GetR32(ESI)), m1);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+cpu->GetR32(EDI)), m2);
    cpu->Cmps(sizeof(m1));
    EXPECT_FALSE(cpu->IsFlag(OF));
    EXPECT_TRUE(cpu->IsFlag(SF));
    EXPECT_FALSE(cpu->IsFlag(ZF));
    EXPECT_TRUE(cpu->IsFlag(PF));
    EXPECT_TRUE(cpu->IsFlag(CF));
    EXPECT_EQ(cpu->GetR32(EDI), 4);
    EXPECT_EQ(cpu->GetR32(ESI), 1004);
}

TEST_F(CpuTest, CheckRep0){
    auto is_termination_condition = [&](){
        return !cpu->IsFlag(ZF);
    };
    auto step_execute = [&](){
        cpu->Cmps(1);
    };
    cpu->SetR32(ECX, 100);
    cpu->On32bitMode();
    cpu->Rep(step_execute, is_termination_condition);
    EXPECT_EQ(cpu->GetR32(ECX), 0);
}

TEST_F(CpuTest, CheckRep1){
    auto is_termination_condition = [&](){
        return cpu->IsFlag(ZF);
    };
    auto step_execute = [&](){
        cpu->Cmps(1);
    };
    cpu->SetR32(ECX, 100);
    cpu->On32bitMode();
    cpu->Rep(step_execute, is_termination_condition);
    EXPECT_EQ(cpu->GetR32(ECX), 99);
}

TEST_F(CpuTest, CheckRep2){
    auto is_termination_condition = [&](){
        return false;
    };
    auto step_execute = [&](){
        cpu->Cmps(1);
    };
    cpu->SetR32(ECX, 1000);
    cpu->On32bitMode();
    cpu->Rep(step_execute, is_termination_condition);
    EXPECT_EQ(cpu->GetR32(ECX), 0);
}

TEST_F(CpuTest, CheckRep3){
    auto is_termination_condition = [&](){
        return true;
    };
    auto step_execute = [&](){
        cpu->Cmps(1);
    };
    cpu->SetR32(ECX, 1000);
    cpu->On32bitMode();
    cpu->Rep(step_execute, is_termination_condition);
    EXPECT_EQ(cpu->GetR32(ECX), 999);
}

TEST_F(CpuTest, CheckMovs0){
    uint8_t data = 0xFA;
    uint32_t src_offset_addr  = 0;
    uint32_t dest_offset_addr = 10000;
    cpu->On32bitMode();
    cpu->ClearFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read8(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr+1);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr+1);
}

TEST_F(CpuTest, CheckMovs1){
    uint16_t data = 0x12FA;
    uint32_t src_offset_addr  = 0;
    uint32_t dest_offset_addr = 10000;
    cpu->On32bitMode();
    cpu->ClearFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read16(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr+2);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr+2);
}

TEST_F(CpuTest, CheckMovs2){
    uint32_t data = 0xF2DC12FA;
    uint32_t src_offset_addr  = 0;
    uint32_t dest_offset_addr = 10000;
    cpu->On32bitMode();
    cpu->ClearFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read32(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr+4);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr+4);
}

TEST_F(CpuTest, CheckMovs3){
    uint64_t data = 0;
    EXPECT_THROW(cpu->Movs(sizeof(data)), runtime_error);
}

TEST_F(CpuTest, CheckMovs4){
    uint8_t data = 0xFA;
    uint32_t src_offset_addr  = 10000;
    uint32_t dest_offset_addr = 20000;
    cpu->On32bitMode();
    cpu->SetFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read8(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr-1);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr-1);
}

TEST_F(CpuTest, CheckMovs5){
    uint16_t data = 0x32FA;
    uint32_t src_offset_addr  = 10000;
    uint32_t dest_offset_addr = 20000;
    cpu->On32bitMode();
    cpu->SetFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read16(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr-2);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr-2);
}

TEST_F(CpuTest, CheckMovs6){
    uint32_t data = 0xAD1232FA;
    uint32_t src_offset_addr  = 10000;
    uint32_t dest_offset_addr = 20000;
    cpu->On32bitMode();
    cpu->SetFlag(DF);
    cpu->SetR32(ESI, src_offset_addr);
    cpu->SetR32(EDI, dest_offset_addr);
    memory->Write(cpu->GetPhysicalAddr(cpu->GetBaseAddr(DS)+src_offset_addr), data);
    cpu->Movs(sizeof(data));
    EXPECT_EQ(memory->Read32(cpu->GetPhysicalAddr(cpu->GetBaseAddr(ES)+dest_offset_addr)), data);
    EXPECT_EQ(cpu->GetR32(ESI), src_offset_addr-4);
    EXPECT_EQ(cpu->GetR32(EDI), dest_offset_addr-4);
}