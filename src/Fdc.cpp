#include "Fdc.h"
#include "IoSpace.h"
#include "Gui.h"
using namespace std;

#define SECTOR_SIZE 512

Fdc::Fdc(const char* const file_name):IoDevice(){
    this->msr.flgs.rqm = 1;
    this->fdc_mode = FDC_IDLE_MODE;
    FILE* disk_image_stream = NULL;
    disk_image_stream = fopen(file_name, "rb");
    if(disk_image_stream==NULL)this->Error("can`t open %s at Fdc::Fdc\n", file_name);
    this->buff = (uint8_t*)malloc(FLOPPY_DISK_SIZE);
    fread(this->buff, 1, FLOPPY_DISK_SIZE, disk_image_stream);
    fclose(disk_image_stream);
}

void Fdc::ProcessCommand(uint8_t command){
    switch (command){
        default:
            this->Error("Not implemented: command = 0x%02X at Fdc::ProcessCommand", command);
            break;
    }
}

//未実装のコマンドタイプをキャッチするために、以下の実装をしている。
FDC_COMMAND_TYPE Fdc::GetFdcCommandType(uint8_t data){
    switch (data){
        case FDC_COMMAND_CHECK_INT:
            return FDC_COMMAND_CHECK_INT;
        case FDC_CMD_SPECIFY:
            return FDC_CMD_SPECIFY;
        case FDC_COMMAND_CALIBRATE: 
            return FDC_COMMAND_CALIBRATE;
        case FDC_COMMAND_READ_SECTOR:
            return FDC_COMMAND_READ_SECTOR;
        case FDC_COMMAND_WRITE_SECTOR:
            return FDC_COMMAND_WRITE_SECTOR;
        default:
            this->Error("Not implemented: fdc_command_type=%02X at Fdc::GetFdcCommandType", data);
    }
}

void Fdc::ProcessCommandForOut8(uint8_t data){
    switch (this->fdc_mode){
        case FDC_IDLE_MODE:
            this->count_in_command_mode = 0;
            this->count_in_result_mode  = 0;
            this->fdc_mode = FDC_COMMAND_MODE;
            this->data_fifo = data;
            this->fdc_command_type = this->GetFdcCommandType(0x0F&this->data_fifo);
            break;
        case FDC_COMMAND_MODE:
            switch(this->fdc_command_type){
                case FDC_CMD_SPECIFY:
                    this->count_in_command_mode++;
                    if(this->count_in_command_mode==2){
                        this->count_in_command_mode = 0;
                        this->fdc_mode = FDC_IDLE_MODE;
                    }
                    break;
                case FDC_COMMAND_CALIBRATE:
                    this->count_in_command_mode = 0;
                    this->fdc_mode = FDC_IDLE_MODE;
                    this->fifo->Push(0);
                    break;
                case FDC_COMMAND_WRITE_SECTOR:
                case FDC_COMMAND_READ_SECTOR:
                    this->count_in_command_mode++;
                    if(this->count_in_command_mode==1){
                        //何もしない
                    }else if(this->count_in_command_mode==2){
                        this->track = data;
                    }else if(this->count_in_command_mode==3){
                        this->head = data;   
                    }else if(this->count_in_command_mode==4){
                        this->sector = data;
                    }else if(this->count_in_command_mode==5){
                        //何もしない
                    }else if(this->count_in_command_mode==6){
                        //何もしない
                    }else if(this->count_in_command_mode==7){
                        //何もしない
                    }else if(this->count_in_command_mode==8){
                        this->count_in_command_mode = 0;
                        this->fdc_mode = FDC_EXECUTION_MODE;//読み込み開始
                        this->fifo->Push(0);
                    }
                    break;
                default:    
                    this->Error("Not implemented: fdc_command_tyep=%d at Fdc::ProcessCommandForOut8", this->fdc_command_type);
            }
            break;
        case FDC_EXECUTION_MODE:
            switch (this->fdc_command_type){
                static int count_in_fdc_in_execution_write_sector=0;
                case FDC_COMMAND_WRITE_SECTOR:
                    this->buff[this->head*18*SECTOR_SIZE+this->track*18*2*SECTOR_SIZE+(this->sector-1)*SECTOR_SIZE+count_in_fdc_in_execution_write_sector] = data;
                    count_in_fdc_in_execution_write_sector++;
                    if(count_in_fdc_in_execution_write_sector==SECTOR_SIZE){
                        count_in_fdc_in_execution_write_sector = 0;
                        this->fdc_mode = FDC_RESULT_MODE;
                    }
                    break;
                default:
                    this->Error("Not implemented: fdc_command_type=%d at Fdc::ProcessCommandForIn8", this->fdc_command_type);
            }
            break;
        default:
            this->Error("Not implemented: fdc_mode=%d Fdc::ProcessCommandForOut8", this->fdc_mode);
    }   
}

void Fdc::Out8(const uint16_t addr, const uint8_t data){
    this->gui->SoundFdc();
    switch (addr){
        case FDC_CONFIGURATION_CONTROL_REGISTER:
            break;
        case FDC_DIGITAL_OUTPUT_REGISTER:
            this->dor.raw = data;
            if(this->dor.flgs.reset){
                this->fdc_mode = FDC_IDLE_MODE;
                this->fifo->Push(0);//値は適当、OS側では使用しない。
            }
            break;
        case FDC_DATA_FIFO:
            this->ProcessCommandForOut8(data);
            break;
        default:
            this->Error("Not implemented: addr=0x%04X at Fdc::Out8", addr);
    }
}

uint8_t Fdc::ProcessCommandForIn8(){
    uint8_t data;
    switch (this->fdc_mode){
        case FDC_COMMAND_MODE:
            switch (this->fdc_command_type){
                case FDC_COMMAND_CHECK_INT:
                    data = 0x00;
                    this->count_in_command_mode++;
                    if(this->count_in_command_mode==2){
                        this->count_in_command_mode = 0;
                        this->fdc_mode = FDC_IDLE_MODE;
                    }
                    break;
                default:
                    this->Error("Not implemented: fdc_command_type=%d at Fdc::ProcessCommandForIn8", this->fdc_command_type);
            }
            break;
        case FDC_EXECUTION_MODE:
            switch (this->fdc_command_type){
                static int count_in_fdc_in_execution_read_sector=0;
                case FDC_COMMAND_READ_SECTOR:
                    data = this->buff[this->head*18*SECTOR_SIZE+this->track*18*2*SECTOR_SIZE+(this->sector-1)*SECTOR_SIZE+count_in_fdc_in_execution_read_sector];
                    //fprintf(stderr, "head=%d, track=%d, sector=%d ,data=%02X\n", head, track, sector, data);
                    count_in_fdc_in_execution_read_sector++;
                    if(count_in_fdc_in_execution_read_sector==SECTOR_SIZE){
                        count_in_fdc_in_execution_read_sector = 0;
                        this->fdc_mode = FDC_RESULT_MODE;
                    }
                    break;
                default:
                    this->Error("Not implemented: fdc_command_type=%d at Fdc::ProcessCommandForIn8", this->fdc_command_type);
            }
            break;
        case FDC_RESULT_MODE:
            switch(this->fdc_command_type){
                case FDC_COMMAND_WRITE_SECTOR:
                case FDC_COMMAND_READ_SECTOR:
                    this->count_in_result_mode++;
                    data = 0x00;
                    if(this->count_in_result_mode==7){
                        this->fdc_mode = FDC_IDLE_MODE;
                    }
                    break;
                default:    
                    this->Error("Not implemented: fdc_command_tyep=%d at Fdc::ProcessCommandForIn8", this->fdc_command_type);
            }
            break;
        default:
            this->Error("Not implemented: fdc_cmode=%d at Fdc::ProcessCommandForIn8", this->fdc_mode);
    }
    return data;
}

uint8_t Fdc::In8(const uint16_t addr){
    this->gui->SoundFdc();
    uint8_t data;
    switch(addr){
        case FDC_DATA_FIFO:
            this->ProcessCommandForIn8();
            break;
        case FDC_MAIN_STATUS_REGISTER:
            data = this->msr.raw;
            break;
        default:
            this->Error("Not implemented: io_addr=0x%04X at Fdc::In8", addr);
    }
    return data;
}


int Fdc::IsEmpty(){
    if(!this->fifo->IsEmpty()){
        this->fifo->Pop();//OS側では使用しないので、Pop
        return 0x06;
    }
    return -1;
}
