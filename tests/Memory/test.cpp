#include "../include/Memory.h"

bool AssertEqual(int expected, int actual){
    if(expected!=actual){
        std::cerr << "Expected " << expected << ", but got " << actual << std::endl;
        return false;
    }
    return true;
}

bool AssertEqualBool(bool expected, bool actual){
    if(expected!=actual){
        std::cerr << "Expected " << (expected?"true":"false") << ", but got " << (actual?"true":"false") << std::endl;
        return false;
    }
    return true;
}

int main(){
    {   //Test1
        std::cerr << "Test1" << std::endl;
        const int MEM_SIZE = 10;
        //サイズチェック
        Memory memory(MEM_SIZE);
        if(!AssertEqual(memory.GetMemSize(), MEM_SIZE)){
            std::cerr << "MEMORY TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }

        //読み書きチェック
        memory.Write(0, (uint8_t)0xFF);
        if(!AssertEqual(memory.Read8(0), 0xFF)){
            std::cerr << "MEMORY TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }

        //範囲外アクセスチェック
        {
            uint8_t data = 0xFA;
            memory.Write(MEM_SIZE, data);
            if(!AssertEqual(memory.Read8(MEM_SIZE), 0)){
                std::cerr << "MEMORY TEST FAILED !!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        {
            uint16_t data = 0xFAFB;
            memory.Write(MEM_SIZE, data);
            if(!AssertEqual(memory.Read16(MEM_SIZE), 0)){
                std::cerr << "MEMORY TEST FAILED !!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        {
            uint32_t data = 0xFAFDFAFB;
            memory.Write(MEM_SIZE, data);
            if(!AssertEqual(memory.Read32(MEM_SIZE), 0)){
                std::cerr << "MEMORY TEST FAILED !!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        //buffの要素の番地取得テスト
        if(!AssertEqualBool(true, NULL==memory.GetPointer(MEM_SIZE))){
            std::cerr << "MEMORY TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }  
        if(!AssertEqualBool(true, NULL!=memory.GetPointer(0))){
            std::cerr << "MEMORY TEST FAILED !!" << std::endl;
            exit(EXIT_FAILURE);
        }  
    }

    std::cerr << "MEMORY TEST SUCCESS !!" << std::endl;
}