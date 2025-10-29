#include <stdlib.h>
#include "util.h"

DEFINE_READ_DATA(uint8_t , byte)
DEFINE_READ_DATA(uint16_t, word)
DEFINE_READ_DATA(uint32_t, dword)
DEFINE_READ_DATA(uint64_t, qword)

DEFINE_WRITE_DATA(uint8_t , byte)
DEFINE_WRITE_DATA(uint16_t, word)
DEFINE_WRITE_DATA(uint32_t, dword)
DEFINE_WRITE_DATA(uint64_t, qword)
