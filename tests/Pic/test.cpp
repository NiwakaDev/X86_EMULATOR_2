#include "../../include/Pic.h"
#include "../../include/IoDevice.h"
#include <memory>
#include <gtest/gtest.h>
using namespace std;

TEST(PicTest, CheckPic_RunTimeError0){
    IoDevice** io_devices = NULL;
    EXPECT_THROW(Pic pic(io_devices), runtime_error);
}

TEST(PicTest, CheckPic_RunTimeError1){
    IoDevice* io_devices[16];
    EXPECT_NO_THROW(Pic pic(io_devices));
}

TEST(PicTest, CheckOut8_RunTimeError_0){
    IoDevice* io_devices[16];
    auto pic = make_unique<Pic>(io_devices);
    uint16_t addr = 0;
    uint8_t  data = 0;
    EXPECT_THROW(pic->Out8(addr, data), runtime_error);
}

TEST(PicTest, CheckIn8_RunTimeError_0){
    IoDevice* io_devices[16];
    auto pic = make_unique<Pic>(io_devices);
    uint16_t addr = 0;
    EXPECT_THROW(pic->In8(addr), runtime_error);
}
