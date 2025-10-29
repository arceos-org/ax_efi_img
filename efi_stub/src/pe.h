/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_PE_H
#define __LINUX_PE_H

#define LINUX_EFISTUB_MAJOR_VERSION		0x3
#define LINUX_EFISTUB_MINOR_VERSION		0x0

#define MZ_MAGIC    0x5a4d	    /* "MZ" */

#define PE_MAGIC    0x00004550	/* "PE\0\0" */
#define PE_OPT_MAGIC_PE32PLUS	0x020b

#define	IMAGE_FILE_MACHINE_RISCV64	0x5064

/* flags */
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200

#define IMAGE_SUBSYSTEM_EFI_APPLICATION     10
#define IMAGE_DLL_CHARACTERISTICS_NX_COMPAT 0x0100

#define IMAGE_SCN_CNT_CODE	            0x00000020 /* .text */
#define IMAGE_SCN_CNT_INITIALIZED_DATA  0x00000040 /* .data */

#define IMAGE_SCN_MEM_EXECUTE	0x20000000 /* can be executed as code */
#define IMAGE_SCN_MEM_READ	0x40000000 /* readable */
#define IMAGE_SCN_MEM_WRITE	0x80000000 /* writeable */

#endif /* __LINUX_PE_H */
