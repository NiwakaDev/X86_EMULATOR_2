#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>

#define CODE_SIZE 1000

void Function(){

}

void Write(uint32_t data, uint8_t* code, int* pc){
    uint8_t* p = (uint8_t*)&data;
    for(int i=0; i<sizeof(data); i++){
        code[*pc+i] = p[i];
    }
    *pc = *pc + sizeof(data);
}

int main(){
    int pc = 0;
    uint8_t* code = mmap(0, CODE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    code[pc++]    = 0x60;       //pushad
    code[pc++]    = 0xB8+7;     //mov esi, imm32
    Write(Function, code, &pc); //imm32=Function
    code[pc++]    = 0xFF;       //call rm32
    code[pc++]    = 0x16;       //rm32=[esi]
    code[pc++]    = 0x61;       //popad
    code[pc++]    = 0xC3;       //ret
    //void (*func)()= (void (*)()) code;
    //func();
    FILE* fp = fopen("binary", "wb+");
    fwrite(code, 1, pc, fp);
    fclose(fp);
}