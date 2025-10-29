/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _ASM_RISCV_IMAGE_H
#define _ASM_RISCV_IMAGE_H

#define RISCV_IMAGE_MAGIC	"RISCV\0\0\0"
#define RISCV_IMAGE_MAGIC2	"RSC\x05"

#define RISCV_IMAGE_FLAG_BE_SHIFT	0
#define RISCV_IMAGE_FLAG_BE_MASK	0x1

#define RISCV_IMAGE_FLAG_LE		0
#define RISCV_IMAGE_FLAG_BE		1

#ifdef CONFIG_CPU_BIG_ENDIAN
#error conversion of header fields to LE not yet implemented
#else
#define __HEAD_FLAG_BE		RISCV_IMAGE_FLAG_LE
#endif

#define __HEAD_FLAG(field)	(__HEAD_FLAG_##field << \
				RISCV_IMAGE_FLAG_##field##_SHIFT)

#define __HEAD_FLAGS		(__HEAD_FLAG(BE))

#define RISCV_HEADER_VERSION_MAJOR 0
#define RISCV_HEADER_VERSION_MINOR 2

#define RISCV_HEADER_VERSION (RISCV_HEADER_VERSION_MAJOR << 16 | \
			      RISCV_HEADER_VERSION_MINOR)

#endif /* _ASM_RISCV_IMAGE_H */
