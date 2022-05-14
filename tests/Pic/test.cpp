#include "../include/Pic.h"

int main(){
    //存在しないデバイスにアクセスしたときに、例外が発生するかのテスト
    {
        Pic pic(NULL);
        try{
            pic.Out8(0, 0);
        }catch(const char* message){
            std::cerr << message << std::endl;
            std::cerr << "PIC TEST SUCCESS !!" << std::endl;
            exit(EXIT_SUCCESS);
        }
    }
    std::cerr << "PIC TEST FAILED !!" << std::endl;
}