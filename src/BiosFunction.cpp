#include "BiosFunction.h"
#include "Cpu.h"
#include "Memory.h"

BiosFunction::BiosFunction(){

}

void BiosFunction::Run(Cpu* cpu, Memory* mem){
    this->Error("Not implemented: BiosFunction::Run");
}


VideoFunction::VideoFunction():BiosFunction(){
    this->function_name = "VideoFunction";
}

void VideoFunction::Run(Cpu *cpu, Memory* mem){
    unsigned char mode;
    mode = cpu->GetR8H(EAX);
    return;
    switch(mode){
        case 0x00:
            //何もせずリターン。本当の実装
            //http://oswiki.osask.jp/?%28AT%29BIOS
            return;
        default:
            this->Error("Not implemented at VideoFunction::Run");
    }
    unsigned char color;
    unsigned char ascii_code;
    
    int bright;
    int len;
    unsigned char data;
    color      = ((uint8_t)cpu->GetR32(EBX))&0x0F;
    ascii_code = cpu->GetR8L(EAX);
    color = this->color_table[color&0x07];
    if(color&0x08){
        bright = 1;
    }else{
        bright = 0;
    }
    fprintf(stderr, "%c", ascii_code);
}

FloppyFunction::FloppyFunction(char* file_name):BiosFunction(){
    this->function_name = "FloppyFunction";
    this->Init(file_name);
}

void FloppyFunction::Init(char* file_name){
    this->disk_img_stream = fopen(file_name, "rb");
    if(this->disk_img_stream==NULL){
        this->Error("can`t open %s at FloppyFunction::Init\n", file_name);
    }
    this->buff = (uint8_t*)malloc(this->floppy_size);
    fread(this->buff, 1, this->floppy_size, this->disk_img_stream);
}

void FloppyFunction::Run(Cpu* cpu, Memory* mem){
    uint8_t ah;
    ah = cpu->GetR8H(EAX);
    switch (ah){
        case 0x02:
            this->Read(cpu, mem);
            break;
        default:
            this->Error("Not implemented: ah=%02X at FloppyFunction::Run\n", ah);
            break;
    }
}

void FloppyFunction::Read(Cpu* cpu, Memory* mem){
    static int cnt=0;
    //this->Error("stopped at FloppyFunction::Read");
    uint32_t buff_addr;
    uint8_t data;
    this->es = (uint32_t)cpu->GetR16(ES);
    this->bx = cpu->GetR16(EBX);
    this->drive_number = (uint32_t)cpu->GetR8L(EDX);
    this->head_number  = (uint32_t)cpu->GetR8H(EDX);
    this->sector_number = 0x0000003F&((uint32_t)cpu->GetR8L(ECX));
    this->cylinder_number = (uint32_t)cpu->GetR8H(ECX);
    if(this->sector_number==0x00 || this->sector_number>0x12){
        fprintf(stderr, "sector_numberは0x01~0x12の範囲内で指定してください(sector_number=%d) at FloppyFunction::ExecuteSelf\n", this->sector_number);
    }  
    this->processed_sector_number = (uint32_t)cpu->GetR8L(EAX);

    buff_addr = this->es*16 + this->bx;
    for(int i=0; i<this->processed_sector_number; i++){
        for(int j=0; j<SECTOR_SIZE; j++){
            if(buff_addr+j+i*SECTOR_SIZE==0xc220){
                buff = buff;
            }
            data = (uint8_t)this->buff[this->head_number*18*SECTOR_SIZE+this->cylinder_number*18*2*SECTOR_SIZE+(this->sector_number-1)*SECTOR_SIZE+j];
            mem->Write(buff_addr+j+i*SECTOR_SIZE, data);
        }
    }

    cpu->ClearFlag(CF);//エラーなし
}

KeyFunction::KeyFunction():BiosFunction(){
    this->function_name = "KeyFunction";
}

void KeyFunction::Run(Cpu* cpu, Memory* mem){
    uint8_t ah;
    uint8_t al;
    ah = cpu->GetR8H(EAX);

    switch (ah){
        case 0x02:
            al = 0x00;
            cpu->SetR8L(EAX, al);
            return;
        default:
            this->Error("Not implemented: ah = 0x%02X at KeyFunction::ExecuteSelf", ah);
            break;
    }
}