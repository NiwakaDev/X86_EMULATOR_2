#include "BiosFunction.h"
#include "Cpu.h"
#include "Memory.h"
#include "Vga.h"
#include "Kbc.h"
using namespace std;

BiosFunction::BiosFunction(){

}

void BiosFunction::Run(Cpu* cpu, Memory* mem){
    this->Error("Not implemented: BiosFunction::Run");
}

MemoryFunction::MemoryFunction():BiosFunction(){
    this->function_name = "MemoryFunction";
}

void MemoryFunction::Run(Cpu *cpu, Memory* mem){
    cpu->SetR16(EAX, 640);
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
    uint8_t ah = cpu->GetR8H(EAX);
    if(ah!=0x4F){
        //VGAサービス
        static int row = 0;
        static int col = 0;
        static bool stop = false;
        static int cnt = 0;
        switch(ah){
            case 0x00:
                if((mode&0x00FF)==0x13){
                    this->vga->SetInfo(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_VRAM_START_ADDR);
                }else{
                    this->Error("Not implemented: video mode=0x%02X at VideoFunction::Run", (uint8_t)(mode&0x00FF));
                }
                return;
            case 0x01:
                //本来はカーソル位置を指定するが、今は無視
                return;
            case 0x13:
                this->vga->SetInfo(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_VRAM_START_ADDR);
                return;
            case 0x0E:
                if(stop)return;
                ascii_code = cpu->GetR8L(EAX);
                this->vga->SetText(ascii_code, col, row);
                col++;
                if(col==80){
                    row++;
                    col = 0;
                }
                cnt++;
                if(cnt==1000){
                    stop = true;
                }
                return;
            case 0x03:
            case 0x08:
            case 0x0F:
            case 0x12:
                break;//とりあえず無視
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
    switch (ah) { // ORIGINAL EXTENSIONS
        case 0xfe: // hopen
            this->Error("Not implemented: ah=%02X at FloppyFunction::Run\n", ah);
            break;
        case 0xff: // hcopy
            this->Error("Not implemented: ah=%02X at FloppyFunction::Run\n", ah);
            break;
    }
    if (cpu->GetR8L(EDX) >= 1) {//ドライブ番号は0しかない。フロッピーだけ
        cpu->SetFlag(CF);
        cpu->SetR8H(EAX, 1);
        return;
    }
    switch (ah){
        case 0x00: 
            cpu->SetR8H(EAX, 0x00);
            cpu->ClearFlag(CF);
            return;
        case 0x02:
            this->Read(cpu, mem);
            break;
        case 0x08: 
            cpu->SetR16(EAX, 0x0000);
            cpu->SetR8L(EBX, 4);
            cpu->SetR8H(ECX, 0x4F);
            cpu->SetR8L(ECX, 0x12);
            cpu->SetR8H(EDX, 1);
            cpu->SetR8L(EDX, 1);
            cpu->SetR16(ES, 0);
            cpu->SetR16(EDI, 0);
            cpu->ClearFlag(CF);
            break;
        case 0x15: // get disk type
            cpu->SetR8H(EAX, 1);
            cpu->ClearFlag(CF);
            return;
        default:
            this->Error("Not implemented: ah=%02X at FloppyFunction::Run\n", ah);
            break;
    }
}

void FloppyFunction::Read(Cpu* cpu, Memory* mem){
    static int total=0;
    static int cnt=0;
    uint32_t buff_addr;
    uint8_t data;
    this->es = (uint32_t)cpu->GetR16(ES);
    this->bx = cpu->GetR16(EBX);
    this->drive_number = (uint32_t)cpu->GetR8L(EDX);
    this->head_number  = (uint32_t)cpu->GetR8H(EDX);
    this->sector_number = 0x0000003F&((uint32_t)cpu->GetR8L(ECX));
    this->cylinder_number = (uint32_t)cpu->GetR8H(ECX);
    this->cylinder_number = ((((uint16_t)cpu->GetR8L(ECX))&0xC0)<<2)|this->cylinder_number;
    this->processed_sector_number = (uint32_t)cpu->GetR8L(EAX);
    buff_addr = this->es*16 + this->bx;
    total = total + 512;
    cnt++;
    for(int i=0; i<this->processed_sector_number; i++){
        for(int j=0; j<SECTOR_SIZE; j++){
            data = (uint8_t)this->buff[this->head_number*18*SECTOR_SIZE+this->cylinder_number*18*2*SECTOR_SIZE+((this->sector_number+i)-1)*SECTOR_SIZE+j];
            mem->Write(buff_addr+j+i*SECTOR_SIZE, data);
        }
    }
    cpu->SetR8H(EAX, 0x00);
    cpu->ClearFlag(CF);//エラーなし
}

EquipmentListFunction::EquipmentListFunction():BiosFunction(){
    this->function_name = "EquipmentListFunction";
}

void EquipmentListFunction::Run(Cpu *cpu, Memory* mem){
    cpu->SetR16(EAX, 1);
}

KeyFunction::KeyFunction(Kbc* kbc):BiosFunction(){
    this->function_name = "KeyFunction";
    this->kbc           = kbc;
}

uint16_t KeyFunction::Decode(uint16_t scan_code){
    uint16_t decoded_code;
    switch (scan_code){
        case KEY_CODE_ENTER:
            decoded_code = ((KEY_CODE_ENTER)<<BYTE)|0x0d;
            break;
        case KEY_CODE_D:
            decoded_code = ((KEY_CODE_D)<<BYTE)|'d';
            break;
        case KEY_CODE_I:
            decoded_code = ((KEY_CODE_I)<<BYTE)|'i';
            break;
        case KEY_CODE_R:
            decoded_code = ((KEY_CODE_R)<<BYTE)|'r';
            break;
        default:
            this->Error("Not implemented: scan_code=%04X at KeyFunction::Decode\n", scan_code);
            break;
    }
    return decoded_code;
}

//fifoにプッシュすべき値
//AL == ASCIIコード, AH == キーボードスキャンコード
//キーボードスキャンコード : http://oswiki.osask.jp/?%28AT%29keyboard
void KeyFunction::Run(Cpu* cpu, Memory* mem){
    uint8_t ah;
    uint8_t al;
    ah = cpu->GetR8H(EAX);
    uint16_t ch;
    int d;
    switch (ah){
        case 0x00:
            while(this->kbc->IsEmpty()==-1){

            }
            ch = this->Decode(this->kbc->Pop());
            cpu->SetR16(EAX, ch);
            break;
        case 0x01:
            if(this->kbc->IsEmpty()==-1){
                cpu->SetFlag(ZF);
                //空の場合は、8086runのbios.cppの実装だとAXに0を入れている。
                cpu->SetR16(EAX, 0x0000);
                break;
            }
            //KEY_CODE_BREAKに該当する場合はPopして、渡さないようにする。
            //KEY_CODE_MAKEとKEY_CODE_BREAKの両方がリアルモードで渡されると
            //2回文字が表示されてしまうから。
            if((this->kbc->Front()&KEY_CODE_BREAK)!=0){
                this->kbc->Pop();
                cpu->SetFlag(ZF);
                cpu->SetR16(EAX, 0x0000);
                break;
            }
            //Popでない理由は、この機能ではバッファに残したままにしておく仕様だから。
            ch = this->Decode(this->kbc->Front());
            cpu->ClearFlag(ZF);
            cpu->SetR16(EAX, ch);
            break;
        case 0x02:
            al = 0x00;
            cpu->SetR8L(EAX, al);
            return;
        default:
            this->Error("Not implemented: ah = 0x%02X at KeyFunction::Run", ah);
            break;
    }
}

TimerFunction::TimerFunction():BiosFunction(){
    this->function_name = "TimerFunction";
}

//リアルモードで使用するTimerFunctionの値は全部固定(デバッグが大変になるから。)
//8086runを参考に実装予定
void TimerFunction::Run(Cpu* cpu, Memory* mem){
    uint8_t ah;
    uint8_t al;
    ah = cpu->GetR8H(EAX);
    switch (ah){
        case 0x00:
            mem->Write(0x470, (uint8_t)0x00);
            cpu->SetR8L(EAX, 0x00);
            cpu->SetR16(ECX, 0x00);
            cpu->SetR16(EDX, 0x00);
            break;
        case 0x01: 
            mem->Write(0x46e, (uint16_t)0x00);
            mem->Write(0x46c, (uint16_t)0x00);
            mem->Write(0x470, (uint8_t)0x00);
            break;
        case 0x02://8086runのbios.cppを参考
            cpu->SetR8H(ECX, 10);
            cpu->SetR8L(ECX, 10);
            cpu->SetR8H(EDX, 10);
            cpu->SetR8L(EDX, 1);
            cpu->ClearFlag(CF);
            break;
        case 0x04:
            cpu->SetR8H(ECX, 10);
            cpu->SetR8L(ECX, 10);
            cpu->SetR8H(EDX, 10);
            cpu->SetR8L(EDX, 1);
            cpu->ClearFlag(CF);
            break;
        case 0x03: 
        case 0x05: 
            return; 
        default:
            this->Error("Not implemented: ah = 0x%02X at TimerFunction::Run", ah);
            break;
    }
}