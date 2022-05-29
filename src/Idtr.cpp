#include "Idtr.h"
using namespace std;

Idtr::Idtr(string name, uint16_t limit, uint32_t base)
    : DescriptorTableRegister(name, limit, base) {}