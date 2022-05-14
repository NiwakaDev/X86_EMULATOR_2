#include "common.h"
#include "Addr.h"

bool AssertEqual(uint32_t expected, uint32_t actual){
    if(expected!=actual){
        std::cerr << "Expected " << expected << ", but got " << actual << std::endl;
        return false;
    }
    return true;
}

int main(){
    {
        Addr addr = Addr(10);
        if(!AssertEqual((uint32_t)addr, 10)){
            std::cerr << "ADDR TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    {
        Addr addr = Addr(0xFFFFFFFF);
        if(!AssertEqual((uint16_t)addr, 0x0000FFFF)){
            std::cerr << "ADDR TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    {
        Addr addr = Addr(0x000000FF);
        if(!AssertEqual((uint8_t)addr, 0x000000FF)){
            std::cerr << "ADDR TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cerr << "ADDR TEST SUCCESS !!" << std::endl;
}