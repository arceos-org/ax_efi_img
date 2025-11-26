/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TYPES_H_
#define _LINUX_TYPES_H_

typedef unsigned long   u64;
typedef unsigned int    u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

typedef unsigned long size_t;

typedef _Bool   bool;

#define UUID_SIZE 16

typedef struct {
    u8 b[UUID_SIZE];
} guid_t;

/**
 * ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define __aligned(x)    __attribute__((__aligned__(x)))

#define __aligned_u64 u64 __attribute__((aligned(8)))

#ifndef NULL
#define NULL    (0)
#endif

#ifdef CONFIG_64BIT
#define BITS_PER_LONG 64
#else
#define BITS_PER_LONG 32
#endif /* CONFIG_64BIT */

#endif /* _LINUX_TYPES_H_ */
