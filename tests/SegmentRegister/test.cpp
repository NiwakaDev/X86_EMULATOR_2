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
    auto MockGetGdtGate = [&](uint16_t data){return &gdt_gate;};
    auto MockGetLdtGate = [&](uint16_t data){return (GdtGate*)NULL;};
    uint16_t mock_data  = 0x1000;
    bool is_protected_mode = true;
    segment_register.Set(mock_data, MockGetGdtGate, MockGetLdtGate, is_protected_mode);
    EXPECT_EQ(segment_register.Is32bitsMode(), false);
    EXPECT_EQ(segment_register.GetLimit(), 0);
    EXPECT_EQ(segment_register.GetBaseAddr(), 0);
    EXPECT_EQ(segment_register.GetDpl(), 0);
}

TEST(SegmentRegisterTest, CheckSetCache1){
    uint16_t selector = 0x0000;
    SegmentRegister segment_register(selector);
    uint16_t limit_low    = 0x0000;
    uint16_t base_low     = 0xFBCD;
    uint8_t  base_mid     = 0x12;
    uint8_t  access_right = 0x60;
    uint8_t limit_high    = 0x40;
    uint8_t base_high     = 0xFA;
    GdtGate gdt_gate = {limit_low, base_low, base_mid, access_right, limit_high, base_high};
    auto MockGetGdtGate = [&](uint16_t data){return &gdt_gate;};
    auto MockGetLdtGate = [&](uint16_t data){return (GdtGate*)NULL;};
    uint16_t mock_data  = 0x1000;
    bool is_protected_mode = true;
    segment_register.Set(mock_data, MockGetGdtGate, MockGetLdtGate, is_protected_mode);
    EXPECT_EQ(segment_register.Is32bitsMode(), true);
    EXPECT_EQ(segment_register.GetLimit(), 0);
    EXPECT_EQ(segment_register.GetBaseAddr(), 0xFA12FBCD);
    EXPECT_EQ(segment_register.GetDpl(), 3);
}

TEST(SegmentRegisterTest, CheckSetCache2){
    uint16_t selector = 0x0000;
    SegmentRegister segment_register(selector);
    uint16_t limit_low    = 0xFFFF;
    uint16_t base_low     = 0xF000;
    uint8_t  base_mid     = 0x12;
    uint8_t  access_right = 0x20;
    uint8_t limit_high    = 0x4F;
    uint8_t base_high     = 0xF0;
    GdtGate gdt_gate = {limit_low, base_low, base_mid, access_right, limit_high, base_high};
    auto MockGetGdtGate = [&](uint16_t data){return &gdt_gate;};
    auto MockGetLdtGate = [&](uint16_t data){return (GdtGate*)NULL;};
    uint16_t mock_data  = 0x1000;
    bool is_protected_mode = true;
    segment_register.Set(mock_data, MockGetGdtGate, MockGetLdtGate, is_protected_mode);
    EXPECT_EQ(segment_register.Is32bitsMode(), true);
    EXPECT_EQ(segment_register.GetLimit(), 0x000FFFFF);
    EXPECT_EQ(segment_register.GetBaseAddr(), 0xF012F000);
    EXPECT_EQ(segment_register.GetDpl(), 1);
}

TEST(SegmentRegisterTest, CheckSetCache3){
    //MEMO : real modeのテスト
    uint16_t selector = 0x1000;
    SegmentRegister segment_register(selector);
    uint16_t limit_low    = 0xFFFF;
    uint16_t base_low     = 0xF000;
    uint8_t  base_mid     = 0x12;
    uint8_t  access_right = 0x20;
    uint8_t limit_high    = 0x4F;
    uint8_t base_high     = 0xF0;
    GdtGate gdt_gate = {limit_low, base_low, base_mid, access_right, limit_high, base_high};
    auto MockGetGdtGate = [&](uint16_t data){return &gdt_gate;};
    auto MockGetLdtGate = [&](uint16_t data){return (GdtGate*)NULL;};
    uint16_t mock_data  = 0x1000;
    bool is_protected_mode = false;
    segment_register.Set(mock_data, MockGetGdtGate, MockGetLdtGate, is_protected_mode);
    EXPECT_EQ(segment_register.Is32bitsMode(), false);
    EXPECT_EQ(segment_register.GetLimit(), 0x0000FFFF);
    EXPECT_EQ(segment_register.GetBaseAddr(), selector*16);
    EXPECT_EQ(segment_register.GetData(), 0x1000);
}
