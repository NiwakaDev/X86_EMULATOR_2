#pragma once
#include "CpuEnum.h"
#include "InstructionHelper.h"
#include "common.h"

class SegmentRegister;
class Memory;
class Instruction;
class Ldtr;
class TaskRegister;
class Emulator;
class DescriptorTableRegister;

// TODO : defineで定義された定数をconstに変更
#define IPL_START_ADDR 0x7c00

#define SIGN_FLG1 0x00000080
#define SIGN_FLG2 0x00008000
#define SIGN_FLG4 0x80000000
#define TSS_TYPE 0x09
#define LDT_TYPE 0x0082
#define SEGMENT_DESC_TYPE_FLG 1 << 4

#define DPL 0x60  // access_rightのDPLの該当部分

namespace CpuHelper {
inline template <typename type>
uint8_t GetDpl(type data) {
  return (data & DPL) >> 5;
}

inline template <typename type>
uint8_t GetRpl(type data) {
  return data & 0x03;
}
}  // namespace CpuHelper

struct _GdtGate {
  uint16_t limit_low, base_low;
  uint8_t base_mid, access_right;
  uint8_t limit_high, base_high;
} __attribute__((__packed__));

struct _IdtGate {
  uint16_t offset_low, selector;
  uint8_t dw_cnt, access_right;
  uint16_t offset_high;
} __attribute__((__packed__));

struct _CallGate {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t param_cnt;
  uint8_t access_right;
  uint16_t offset_high;
} __attribute__((__packed__));

struct _Tss {
  uint32_t backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
  uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  uint32_t es, cs, ss, ds, fs, gs;
  uint32_t ldtr, iomap;
} __attribute__((__packed__));

typedef _GdtGate GdtGate;
typedef _IdtGate IdtGate;
typedef _Tss Tss;
typedef _CallGate CallGate;

const int DB = 0x40;

/***こちらを採用するが、まずは汎用レジスタでうまくいくかどうかを調べる。
struct _Registers{
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
}__attribute__((__packed__));
typedef struct _Registers Registers;
***/

struct _Registers {
  uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
} __attribute__((__packed__));
typedef struct _Registers Registers;

// TODO : 神クラスを何とかして整理
class Cpu {
 public:
  Cpu(std::function<void(Cpu& cpu, Memory& mem, const uint8_t bios_number)>
          bios_callback,
      Memory& mem, std::function<uint8_t(uint16_t addr)> io_in8,
      std::function<void(uint16_t addr, uint8_t data)> io_out8);
  ~Cpu();
  void Run();
  inline void AddEip(uint32_t data);
  inline void SetEip(uint32_t addr);
  uint32_t GetLinearAddrForCodeAccess();
  uint32_t GetLinearAddrForDataAccess(uint32_t offset);
  uint32_t GetLinearStackAddr();
  inline bool IsProtectedMode() const;
  inline bool IsRealMode() const;
  bool Is32bitsMode();
  bool IsStackAddr32();  // true : 32bit addr, false : 16bit
  inline bool IsPrefixAddrSize();
  inline bool IsPrefixOpSize();
  inline void SetR8L(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data);
  inline void SetR8H(GENERAL_PURPOSE_REGISTER32 register_type, uint8_t data);
  inline void SetR8(uint32_t register_type, uint8_t data);
  void SetR16(SEGMENT_REGISTER register_type, uint16_t data);
  inline void SetR16(GENERAL_PURPOSE_REGISTER32 register_type, uint16_t data);
  inline void SetR32(GENERAL_PURPOSE_REGISTER32 register_type, uint32_t data);
  void SetGdtr(uint16_t limit, uint32_t base);
  void SetIdtr(uint16_t limit, uint32_t base);
  void SetLdtr(uint16_t data);
  void SetCr(CONTROL_REGISTER control_register_type, uint32_t data);
  void SetTr(uint16_t selector);
  inline void SetDataSelector(SEGMENT_REGISTER register_type);
  inline void SetEflgs(uint32_t eflgs);
  inline uint8_t GetR8(uint32_t register_type);
  inline uint8_t GetR8L(GENERAL_PURPOSE_REGISTER32 register_type);
  inline uint8_t GetR8H(GENERAL_PURPOSE_REGISTER32 register_type);
  inline uint16_t GetR16(GENERAL_PURPOSE_REGISTER32 register_type);
  uint16_t GetR16(SEGMENT_REGISTER register_type);
  inline uint32_t GetR32(GENERAL_PURPOSE_REGISTER32 register_type);
  inline uint32_t GetEip();
  inline uint32_t GetEflgs();
  uint32_t GetCr(CONTROL_REGISTER control_register_type);
  uint16_t GetLdtr();
  uint32_t GetGdtBaseAddr() const;
  uint32_t GetIdtBase();
  GdtGate* GetGdtGate(uint16_t selector) const;
  GdtGate* GetLdtGate(uint16_t selector) const;
  IdtGate* GetIdtGate(uint16_t selector);
  Tss* GetCurrentTss();
  uint32_t GetBaseAddr(SEGMENT_REGISTER register_type);
  template <typename type>
  type Rcr(type data, type count);
  template <typename type>
  type Inc(type data);
  template <typename type>
  type Dec(type data);
  template <typename type>
  type Adc(type data1, type data2, type carry);
  template <typename type>
  type Add(type data1, type data2);
  template <typename type>
  type Sub(type data1, type data2);
  template <typename type>
  type And(type data1, type data2);
  template <typename type>
  type Or(type data1, type data2);
  template <typename type>
  type Xor(type data1, type data2);
  template <typename type>
  type Sar(type data1, type data2);
  template <typename type>
  type Shr(type data, type count);
  template <typename type>
  type Sal(type data, type count);
  template <typename type>
  type Rcl(type data, type count);
  template <typename type>
  type Ror(type data, type count);
  template <typename type>
  void Stos(type data);
  template <typename type>
  void Scas(type data);
  void Cmps(int operand_size);
  template <typename type>
  void UpdateEflagsForDec(type result, type d1, type d2);
  template <typename type>
  void UpdateEflagsForShr(type result);
  bool IsFlag(EFLAGS_KIND eflags_kind);
  void SetFlag(EFLAGS_KIND eflags_kind);
  void ClearFlag(EFLAGS_KIND eflags_kind);
  template <typename type>
  void UpdateEflagsForAnd(type data);
  template <typename type>
  void UpdateEflagsForUnsignedMul(type data);
  void CallFunctionOnRealMode(uint8_t selector);
  void HandleInterrupt(int irq_num);
  void SaveTask(uint16_t selector);
  void SwitchTask();
  void ShowRegisters();
  uint8_t GetCpl();
  bool CheckPrivilegeLevel();
  void SetRpl(SEGMENT_REGISTER register_type, uint8_t rpl);
  void SetCpl(SEGMENT_REGISTER register_type, uint8_t cpl);
  void On32bitMode();
  bool IsSegmentOverride();
  bool IsBflg(
      SEGMENT_REGISTER register_type);  //セグメントディスクリプタのBフラグ
  void Debug(FILE* f, bool h);
  bool IsException();
  void SetException(int error_code);
  void SetVectorNumber(int vector_number);
  int GetVectorNumber();
  void SetPrefix66();
  uint32_t GetPhysicalAddr(uint32_t linear_addr);
  uint8_t GetIopl();
  uint16_t GetGdtLimit();
  uint8_t GetDpl(uint16_t selector);
  void Push16(uint16_t data);
  void Push32(uint32_t data);
  uint8_t Pop8();
  uint16_t Pop16();
  uint32_t Pop32();

 private:
  std::unique_ptr<Object> obj;
  Registers registers;
  Memory* mem;
  std::function<void(Cpu& cpu, Memory& mem, const uint8_t bios_number)>
      bios_call;
  std::unique_ptr<DescriptorTableRegister> gdtr;
  std::unique_ptr<DescriptorTableRegister> idtr;
  std::unique_ptr<Ldtr> ldtr;
  std::unique_ptr<TaskRegister> task_register;
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SIZE];
  SEGMENT_REGISTER default_code_selector;
  SEGMENT_REGISTER default_data_selector;
  SEGMENT_REGISTER default_stack_selector;
  bool prefix_table[256];
  bool prefix_flgs[256];
  // bool prefix_flgs[PREFIX_FLG_KIND_COUNT];
  //制御レジスタ : 「IA-32 インテル®
  //アーキテクチャソフトウェア・デベロッパーズ・マニュアル下巻：システム・プログラミング・ガイド」2.5節を参照
  union {
    uint32_t raw;
    struct {
      unsigned PE : 1;
      unsigned MP : 1;
      unsigned EM : 1;
      unsigned TS : 1;
      unsigned ET : 1;
      unsigned NE : 1;
      unsigned reserve1 : 10;
      unsigned WP : 1;
      unsigned reserve2 : 1;
      unsigned AM : 1;
      unsigned reserve3 : 10;
      unsigned NW : 1;
      unsigned CD : 1;
      unsigned PG : 1;
    } flgs;
  } cr0;
  uint32_t
      cr2;  // cr2は全体がページフォルトリニアアドレスとなり、unionである必要はない。
  union {
    uint32_t raw;
    struct {
      unsigned reserve1 : 3;
      unsigned pwd : 1;
      unsigned pcd : 1;
      unsigned reserve2 : 7;
      unsigned page_dir_base : 20;
    } flgs;
  } cr3;
  uint32_t eip = IPL_START_ADDR;
  uint32_t* gprs[GENERAL_PURPOSE_REGISTER32_COUNT];
  std::unique_ptr<SegmentRegister> segment_registers[SEGMENT_REGISTER_COUNT];
  union {
    uint32_t raw;
    struct {
      unsigned CF : 1;
      unsigned RESERVED0 : 1;
      unsigned PF : 1;
      unsigned RESERVED1 : 1;
      unsigned AF : 1;
      unsigned RESERVED2 : 1;
      unsigned ZF : 1;
      unsigned SF : 1;
      unsigned TF : 1;
      unsigned IF : 1;
      unsigned DF : 1;
      unsigned OF : 1;
      unsigned IOPL : 2;
      unsigned NT : 1;
      unsigned RESERVED3 : 1;
      unsigned RF : 1;
      unsigned VM : 1;
      unsigned AC : 1;
      unsigned VIF : 1;
      unsigned VIP : 1;
      unsigned ID : 1;
    } flgs;
  } eflags;
  void InitSelector();
  void ResetPrefixFlg();
  void CheckPrefixCode(const Memory& mem);
  bool segment_override;
  bool is_exception_;  //今度からメンバ変数の最後に_をつける
  int vector_number_;
  int error_code_;
  template <typename type>
  void UpdateOF_Add(type result, type d1, type d2);  // TODO : 関数名の変更
  void UpdateOF_Sub(uint32_t result, uint32_t d1, uint32_t d2);
  void UpdateOF_Sub16(uint16_t result, uint16_t d1, uint16_t d2);
  void UpdateOF_Sub8(uint8_t result, uint8_t d1, uint8_t d2);
  template <typename type>
  void UpdateEflagsForSub(type data1, type data2);
  template <typename type>
  void UpdateEflagsForAdd(type d1, type d2);
  template <typename type>
  void UpdateEflagsForInc(type d);
  template <typename type>
  void UpdateEflagsForAdc(type d1, type d2, type c);
  void UpdateZF(uint32_t result);
  void UpdateCF(uint64_t result);
  template <typename type>
  void UpdateCfForSub(type data, int group);
  void UpdatePF(uint8_t result);
  template <typename type>
  void UpdateSF(type data);
};

#include "detail/Cpu.h"