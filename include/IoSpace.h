//このファイルにIOアドレスをどんどん定義する予定(気が向いたら。。。)
enum PIT_REGISTERS{
    PIT_CHANNEL_0             = 0x40,
    PIT_CHANNEL_1             = 0x41,
    PIT_CHANNEL_2             = 0x42,
    PIT_MODE_COMMAND_REGISTER = 0x43,
};

enum FDC_REGISTERS{
    FDC_STATUS_REGISTER_A                 = 0x3F0,
    FDC_STATUS_REGISTER_B                 = 0x3F1,
    FDC_DIGITAL_OUTPUT_REGISTER           = 0x3F2,
    FDC_TAPE_DRIVE_REGISTER               = 0x3F3,
    FDC_MAIN_STATUS_REGISTER              = 0x3F4,
    FDC_DATARATE_SELECT_REGISTER          = 0x3F4,
    FDC_DATA_FIFO                         = 0x3F5,
    FDC_DIGITAL_INPUT_REGISTER            = 0x3F7,
    FDC_CONFIGURATION_CONTROL_REGISTER    = 0x3F7,
};