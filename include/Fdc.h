#pragma once
#include "Fifo.h"
#include "IoDevice.h"
#include "common.h"

const int FLOPPY_DISK_SIZE = 1474560;

class Fdc : public IoDevice {
 public:
  Fdc(std::function<void(uint8_t* buff, int size)> read_callback);
  ~Fdc();
  void Out8(const uint16_t addr, const uint8_t data);
  uint8_t In8(const uint16_t addr);
  int IsEmpty();
  void Push(uint8_t data);
  uint8_t Pop();
  uint8_t Front();
  enum FDC_MODE {
    FDC_IDLE_MODE,
    FDC_COMMAND_MODE,
    FDC_EXECUTION_MODE,
    FDC_RESULT_MODE,
    FDC_MODE_TOTAL
  };

 private:
  Fdc(const Fdc& other);
  void operator=(const Fdc& other);
  FDC_MODE fdc_mode;
  enum FDC_COMMAND_TYPE {
    FDC_CMD_SPECIFY = 0x3,
    FDC_COMMAND_WRITE_SECTOR = 0x5,
    FDC_COMMAND_READ_SECTOR = 0x6,
    FDC_COMMAND_CALIBRATE = 0x7,
    FDC_COMMAND_CHECK_INT = 0x8,
    FDC_COMMAND_TYPE_TOTAL = 5,
  };
  FDC_COMMAND_TYPE fdc_command_type;
  int count_in_command_mode;
  int count_in_result_mode;
  void ProcessCommand(uint8_t command);
  union {
    uint8_t raw;
    struct {
      unsigned drive : 2;
      unsigned reset : 1;
      unsigned mode : 1;
      unsigned motor0 : 1;
      unsigned motor1 : 1;
      unsigned motor2 : 1;
      unsigned motor3 : 1;
    } flgs;
  } dor;
  union {
    uint8_t raw;
    struct {
      unsigned busy0_in_seek_mode : 1;
      unsigned busy1_in_seek_mode : 1;
      unsigned busy2_in_seek_mode : 1;
      unsigned busy3_in_seek_mode : 1;
      unsigned busy_in_read_or_write_command : 1;
      unsigned no_dma_mode : 1;
      unsigned dio : 1;
      unsigned rqm : 1;
    } flgs;
  } msr;
  uint8_t data_fifo;
  FDC_COMMAND_TYPE GetFdcCommandType(uint8_t data);
  uint8_t head;
  uint8_t track;
  uint8_t sector;
  std::unique_ptr<uint8_t[]> buff;
  uint8_t ProcessCommandForIn8();
  void ProcessCommandForOut8(uint8_t data);
  std::unique_ptr<Fifo<uint8_t>> fifo;
  std::unique_ptr<Object> obj;
};