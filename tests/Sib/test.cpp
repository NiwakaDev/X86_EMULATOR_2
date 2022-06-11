#include "../include/Sib.h"
#include <gtest/gtest.h>
#include <memory>
using namespace std;

class SibTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
            sib = make_unique<Sib>();
        }
        unique_ptr<Sib> sib;

};

//sibのフォーマット：
//7~6bit目 : スケール
//5~3bit目 : インデックス
//2~0bit目 : ベース
TEST_F(SibTest, CheckParseSib0){
    uint8_t sib_byte = 0xA5;
    uint8_t mod     = 0;
    sib->ParseSib(sib_byte, mod);
    EXPECT_EQ(sib->GetScale(), 2);
    EXPECT_EQ(sib->GetIndex(), 4);
    EXPECT_EQ(sib->GetBase(), 5);
}
