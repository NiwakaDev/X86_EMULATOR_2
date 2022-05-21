#include "../include/SegmentRegister.h"
#include "../include/Cpu.h"
#include <gtest/gtest.h>

TEST(SegmentRegisterTest, CheckInitSelector0){
    uint16_t selector = 0xFAFA;
    SegmentRegister segment_register(selector);
    EXPECT_EQ(segment_register.GetData(), selector);
}

TEST(SegmentRegisterTest, CheckInitSelector1){
    uint16_t selector = 0x0001;
    SegmentRegister segment_register(selector);
    EXPECT_EQ(segment_register.GetData(), selector);
}

TEST(SegmentRegisterTest, CheckInitCache0){
    uint16_t selector = 0x0000;
    SegmentRegister segment_register(selector);
    EXPECT_EQ(segment_register.GetBaseAddr(), 0);
    EXPECT_EQ(segment_register.Is32bitsMode(), false);
    EXPECT_EQ(segment_register.GetLimit(), 0x0000FFFF);
}

TEST(SegmentRegisterTest, Check32bitsMode0){
    uint16_t selector = 0x0000;
    SegmentRegister segment_register(selector);
    EXPECT_EQ(segment_register.Is32bitsMode(), false);
    segment_register.On32bitMode();
    EXPECT_EQ(segment_register.Is32bitsMode(), true);
}

TEST(SegmentRegisterTest, CheckSetCache0){
    uint16_t selector = 0x0000;
    SegmentRegister segment_register(selector);
    uint16_t limit_low    = 0x0000;
    uint16_t base_low     = 0x0000;
    uint8_t  base_mid     = 0x00;
    uint8_t  access_right = 0x00;
    uint8_t limit_high    = 0x00;
    uint8_t base_high     = 0x00;
    GdtGate gdt_gate = {limit_low, base_low, base_mid, access_right, limit_high, base_high};
    //segment_register.
    //EXPECT_EQ(segment_register.Is32bitsMode(), false);
    //EXPECT_EQ(segment_register.GetLimit(), 0);
    // /EXPECT_EQ(segment_register.GetBaseAddr(), 0);
    //EXPECT_EQ(segment_register.GetDpl(), 0);
}