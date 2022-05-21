#include "../include/SegmentRegister.h"
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