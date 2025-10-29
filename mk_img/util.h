#ifndef _FIXUP_UTIL_
#define _FIXUP_UTIL_

#include <stdio.h>
#include <stdint.h>

typedef uint64_t offset_t;

#define DECLARE_READ_DATA(rtype, name) \
rtype read_##name(FILE *fp, offset_t offset)

#define DEFINE_READ_DATA(rtype, name) \
rtype read_##name(FILE *fp, offset_t offset) \
{ \
    rtype ret = 0; \
    fseek(fp, offset, SEEK_SET); \
    if (fread(&ret, sizeof(ret), 1, fp) != 1) { \
        perror("read error"); \
        exit(-1); \
    } \
    return ret; \
}

#define DECLARE_WRITE_DATA(atype, name) \
int write_##name(FILE *fp, offset_t offset, atype value)

#define DEFINE_WRITE_DATA(atype, name) \
int write_##name(FILE *fp, offset_t offset, atype value) \
{ \
    fseek(fp, offset, SEEK_SET); \
    if (fwrite(&value, sizeof(value), 1, fp) != 1) { \
        perror("write error"); \
        exit(-1); \
    } \
    return 0; \
}

DECLARE_READ_DATA(uint8_t , byte);
DECLARE_READ_DATA(uint16_t, word);
DECLARE_READ_DATA(uint32_t, dword);
DECLARE_READ_DATA(uint64_t, qword);

DECLARE_WRITE_DATA(uint8_t , byte);
DECLARE_WRITE_DATA(uint16_t, word);
DECLARE_WRITE_DATA(uint32_t, dword);
DECLARE_WRITE_DATA(uint64_t, qword);

#define EFI_IMG_SIZE_POS    0x10
#define PECOFF_SIZE_POS     0x90
#define PAYLOAD_SIZE_1_POS  0x150
#define PAYLOAD_SIZE_2_POS  0x158

#define PANIC(MSG) \
do { \
    printf("##############################\n"); \
    printf("[%s:%d]: <%s>\n", __FILE__, __LINE__, __func__); \
    printf("Bug: " MSG ".\n"); \
    printf("##############################\n"); \
    exit(-1); \
} while(0)

#define BUG_ON(COND, MSG) \
do { \
    if (COND) PANIC(MSG); \
} while(0)

#endif /* _FIXUP_UTIL_ */
