#include "../include/Pic.h"
#include "IoDevice.h"
#include <memory>

void Test1(){
    {//存在しないデバイスにアクセスしたときに、例外が発生するかのテスト
        IoDevice* io_devices[16];
        Pic pic(io_devices);
        try{
            pic.Out8(0, 0);
        }catch(const char* message){
            std::cerr << message << std::endl;
            std::cerr << "PIC TEST1 SUCCESS !!" << std::endl;
            exit(EXIT_SUCCESS);
        }
    }
    std::cerr << "PIC TEST1 FAILED !!" << std::endl;   
}

void Test2(){
    {//Picが完全コンストラクタになっているかどうかのテスト
        try{
            std::unique_ptr<Pic> pic = std::make_unique<Pic>((IoDevice**)NULL);
        }catch(const char* message){
            std::cerr << message << std::endl;
            std::cerr << "PIC TEST2 SUCCESS !!" << std::endl;
            exit(EXIT_SUCCESS);
        }
    }
    std::cerr << "PIC TEST2 FAILED !!" << std::endl;
}

int main(){
    Test2();
}