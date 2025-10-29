/* SPDX-License-Identifier: GPL-2.0-only */

#include "image.h"
#include "efi-header.S"

/*
 * EFI Image Header from the entry '_start'.
 */

.globl _start
.balign 4
_start:
    /*
     * This instruction decodes to "MZ" ASCII required by UEFI.
     */
    c.li s4,-13
    j _start_code
    .balign 8
    /* Image load offset(2MB) from start of RAM */
    .dword 0x200000
	/* Effective size of kernel image */
	.dword _end - _start
	.dword __HEAD_FLAGS
	.word RISCV_HEADER_VERSION
	.word 0
	.dword 0
	.ascii RISCV_IMAGE_MAGIC
	.balign 4
	.ascii RISCV_IMAGE_MAGIC2
	.word pe_head_start - _start
pe_head_start:

	__EFI_PE_HEADER

.align 2
_start_code:
