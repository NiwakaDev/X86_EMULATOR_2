#include "BiosFunction.h"
#include "Cpu.h"
#include "Memory.h"
#include "Vga.h"
using namespace std;

BiosFunction::BiosFunction(){

}

void BiosFunction::Run(Cpu* cpu, Memory* mem){
    this->Error("Not implemented: BiosFunction::Run");
}


VideoFunction::VideoFunction(Vga* vga):BiosFunction(){
    this->function_name = "VideoFunction";
    this->vga = vga;
}

//http://oswiki.osask.jp/?%28AT%29BIOS
void VideoFunction::Run(Cpu *cpu, Memory* mem){
    uint16_t mode;
    uint8_t vga_mode;
    uint16_t video_mode;
    uint16_t height;
    uint16_t width;
    uint32_t vram;
    uint8_t ascii_code;
    mode = cpu->GetR16(EAX);
    if(cpu->GetR8H(EAX)!=0x4F){
        //VGAサービス
        vga_mode = (mode>>8);
        switch(vga_mode){
            case 0x00:
                if((mode&0x00FF)==0x13){
                    this->vga->SetInfo(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_VRAM_START_ADDR);
                }else{
                    this->Error("Not implemented: video mode=0x%02X at VideoFunction::Run", (uint8_t)(mode&0x00FF));
                }
                return;
            case 0x13:
                this->vga->SetInfo(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_VRAM_START_ADDR);
                return;
            case 0x0E:
                ascii_code = cpu->GetR8L(EAX);
                fprintf(stderr, "%c", ascii_code);
                break;
            default:
                this->Error("Not implemented: vga_mode=0x%02X at VideoFunction::Run", vga_mode);
        }
    }else{//VESAサービス
        switch(mode){
            case 0x4F02:
                video_mode = cpu->GetR16(EBX);
                height = 400;
                width  = 640;
                vram   = 0xfd000000;
                cpu->SetR8H(EAX, 0x00);
                cpu->SetR8L(EAX, 0x4F);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x28, (uint32_t)vram);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x12, (uint8_t)640);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x14, (uint8_t)400);
                this->vga->SetInfo(width, height, vram);
                return;
            case 0x4F00:
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+4, (uint16_t)0xe004);
                cpu->SetR8H(EAX, 0x00);
                cpu->SetR8L(EAX, 0x4F);
                return;
            case 0x4F01:
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI), (uint16_t)0x00BB);//0x100 :  640 x  400 x 8bitカラー
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x19, (uint8_t)0x08);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x1b, (uint8_t)0x04);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x12, (uint16_t)640);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x14, (uint16_t)400);
                mem->Write(cpu->GetR16(ES)*16+cpu->GetR16(EDI)+0x28, (uint32_t)0xfd000000);
                cpu->SetR8H(EAX, 0x00);
                cpu->SetR8L(EAX, 0x4F);
                return;
            default:
                this->Error("Not implemented: vesa_mode=0x%04X at VideoFunction::Run", mode);
        }
    }
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
        this->Error("invalid sector_number: 0x%02X at FloppyFunction::Read\n", this->sector_number);
    }  
    this->processed_sector_number = (uint32_t)cpu->GetR8L(EAX);

    buff_addr = this->es*16 + this->bx;
    for(int i=0; i<this->processed_sector_number; i++){
        for(int j=0; j<SECTOR_SIZE; j++){
            data = (uint8_t)this->buff[this->head_number*18*SECTOR_SIZE+this->cylinder_number*18*2*SECTOR_SIZE+(this->sector_number-1)*SECTOR_SIZE+j];
            mem->Write(buff_addr+j+i*SECTOR_SIZE, data);
        }
    }
    cnt++;
    cpu->SetR8H(EAX, 0x00);
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