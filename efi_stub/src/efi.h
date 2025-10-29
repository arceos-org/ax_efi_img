/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_EFI_H
#define _LINUX_EFI_H

#include "types.h"

typedef unsigned long efi_status_t;
typedef void *efi_handle_t;
typedef u64 efi_physical_addr_t;
typedef u16 efi_char16_t;		/* UNICODE character */

#if defined(CONFIG_X86_64)
#define __efiapi __attribute__((ms_abi))
#elif defined(CONFIG_X86_32)
#define __efiapi __attribute__((regparm(0)))
#else
#define __efiapi
#endif

#define __packed    __attribute__((__packed__))

struct efi_generic_dev_path {
    u8              type;
    u8              sub_type;
    u16             length;
} __packed;

/*
 * Generic EFI table header
 */
typedef	struct {
	u64 signature;
	u32 revision;
	u32 headersize;
	u32 crc32;
	u32 reserved;
} efi_table_hdr_t;

typedef union efi_simple_text_input_protocol efi_simple_text_input_protocol_t;
typedef union efi_simple_text_output_protocol efi_simple_text_output_protocol_t;

typedef struct {
	efi_table_hdr_t hdr;
	u32 fw_vendor;	/* physical addr of CHAR16 vendor string */
	u32 fw_revision;
	u32 con_in_handle;
	u32 con_in;
	u32 con_out_handle;
	u32 con_out;
	u32 stderr_handle;
	u32 stderr;
	u32 runtime;
	u32 boottime;
	u32 nr_tables;
	u32 tables;
} efi_system_table_32_t;

typedef union efi_runtime_services efi_runtime_services_t; 
typedef union efi_boot_services efi_boot_services_t; 

typedef union {
	struct {
		efi_table_hdr_t hdr;
		unsigned long fw_vendor;	/* physical addr of CHAR16 vendor string */
		u32 fw_revision;
		unsigned long con_in_handle;
		efi_simple_text_input_protocol_t *con_in;
		unsigned long con_out_handle;
		efi_simple_text_output_protocol_t *con_out;
		unsigned long stderr_handle;
		unsigned long stderr;
		efi_runtime_services_t *runtime;
		efi_boot_services_t *boottime;
		unsigned long nr_tables;
		unsigned long tables;
	};
	efi_system_table_32_t mixed_mode;
} efi_system_table_t;

typedef union efi_boot_services efi_boot_services_t;

#define EFI_SUCCESS     0

/*
 * The UEFI spec and EDK2 reference implementation both define EFI_GUID as
 * struct { u32 a; u16; b; u16 c; u8 d[8]; }; and so the implied alignment
 * is 32 bits not 8 bits like our guid_t. In some cases (i.e., on 32-bit ARM),
 * this means that firmware services invoked by the kernel may assume that
 * efi_guid_t* arguments are 32-bit aligned, and use memory accessors that
 * do not tolerate misalignment. So let's set the minimum alignment to 32 bits.
 *
 * Note that the UEFI spec as well as some comments in the EDK2 code base
 * suggest that EFI_GUID should be 64-bit aligned, but this appears to be
 * a mistake, given that no code seems to exist that actually enforces that
 * or relies on it.
 */
typedef guid_t efi_guid_t __aligned(__alignof__(u32));

#define EFI_GUID(a, b, c, d...) (efi_guid_t){ {                 \
    (a) & 0xff, ((a) >> 8) & 0xff, ((a) >> 16) & 0xff, ((a) >> 24) & 0xff,  \
    (b) & 0xff, ((b) >> 8) & 0xff,                      \
    (c) & 0xff, ((c) >> 8) & 0xff, d } }

#define LOADED_IMAGE_PROTOCOL_GUID      EFI_GUID(0x5b1b31a1, 0x9562, 0x11d2,  0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b)

#endif /* _LINUX_EFI_H */
