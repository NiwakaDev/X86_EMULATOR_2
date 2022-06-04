#include "../../include/Object.h"
#include <gtest/gtest.h>
#include <memory>
using namespace std;

class ObjectTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
            object = make_unique<Object>();
        }
        unique_ptr<Object> object;
};

TEST_F(ObjectTest, CheckFormat0){
    EXPECT_EQ(object->Format("Hello"), "Hello");
}

TEST_F(ObjectTest, CheckFormat1){
    EXPECT_EQ(object->Format("World"), "World");
}

TEST_F(ObjectTest, CheckFormat2){
    EXPECT_EQ(object->Format("Hello, %s", "NIWAKA"), "Hello, NIWAKA");
}

TEST_F(ObjectTest, CheckFormat3){
    EXPECT_EQ(object->Format("%s is %s", "NIWAKA", "human"), "NIWAKA is human");
}

TEST_F(ObjectTest, CheckFormat4){
    uint8_t op_code = 0xF2;
    EXPECT_EQ(object->Format("Not implemented: op_code=0x%02X", op_code), "Not implemented: op_code=0xF2");
}

TEST_F(ObjectTest, CheckFormat5){
    uint8_t op_code = 0xF2;
    EXPECT_EQ(object->Format("Not implemented: op_code=0x%08X", op_code), "Not implemented: op_code=0x000000F2");
}

TEST_F(ObjectTest, CheckFormat6){
    uint8_t op_code = 0xF2;
    EXPECT_EQ(object->Format("Not implemented: op_code=0x%08X at %s", op_code, "Cpu::Run"), "Not implemented: op_code=0x000000F2 at Cpu::Run");
}