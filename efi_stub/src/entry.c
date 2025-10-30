// SPDX-License-Identifier: GPL-2.0-only

#include "efistub.h"
#include "rwonce.h"

#define __noreturn __attribute__((__noreturn__))

#define csr_read(csr)                       \
({                              \
    register unsigned long __v;             \
    __asm__ __volatile__ ("csrr %0, " #csr    \
                  : "=r" (__v) :            \
                  : "memory");          \
    __v;                            \
})

#define csr_reset_satp(val)					\
({								\
	unsigned long __v = (unsigned long)(val);		\
	__asm__ __volatile__ ("csrw satp, %0"	\
			      : : "rK" (__v)			\
			      : "memory");			\
})

#define sstatus_set(val)                 \
({                              \
    unsigned long __v = (unsigned long)(val);       \
    __asm__ __volatile__ ("csrs sstatus, %0" \
                  : : "rK" (__v)            \
                  : "memory");          \
})

#define sstatus_clear(val)                 \
({                              \
    unsigned long __v = (unsigned long)(val);       \
    __asm__ __volatile__ ("csrc sstatus, %0" \
                  : : "rK" (__v)            \
                  : "memory");          \
})

#define csr_reset_sie(val)					    \
({								                \
	__asm__ __volatile__ ("csrw sie, zero");    \
})

#define csr_reset_sip(val)					    \
({								                \
	__asm__ __volatile__ ("csrw sip, zero");    \
})

#define SR_SIE      0x00000002UL    /* Supervisor Interrupt Enable */
#define SR_SPIE     0x00000020UL    /* Previous Supervisor IE */
#define SR_SPP      0x00000100UL

#define ARCEOS_BASE     0x80200000

#define PAYLOAD_SIZE_1_POS  0x150

extern char _end[];

const efi_system_table_t *efi_system_table;

typedef void __noreturn (*jump_kernel_func)(unsigned long, unsigned long);

void __noreturn efi_enter_kernel(unsigned long entrypoint, unsigned long fdt,
				 unsigned long fdt_size)
{
    unsigned long hartid = 0;
	jump_kernel_func jump_kernel = (jump_kernel_func)entrypoint;

	/*
	 * Jump to real kernel here with following constraints.
	 * 1. MMU should be disabled.
	 * 2. SIE bit in SSTATUS should be cleared.
	 * 3. SIE&&SIP should be disabled.
	 * 4. a0 should contain hartid
	 * 5. a1 should DT address
	 */
	csr_reset_satp(0);
    sstatus_clear(SR_SIE);
    sstatus_clear(SR_SPIE);
    sstatus_set(SR_SPP);
    csr_reset_sie(0);
    csr_reset_sip(0);
	jump_kernel(hartid, fdt);
}

static u32 get_payload_size(void *image_base)
{
    u32 *p = (u32 *) (image_base + PAYLOAD_SIZE_1_POS);
    return *p;
}

/*
 * EFI entry point for the generic EFI stub used by ARM, arm64, RISC-V and
 * LoongArch. This is the entrypoint that is described in the PE/COFF header
 * of the core kernel.
 */
efi_status_t __efiapi efi_pe_entry(efi_handle_t handle,
				   efi_system_table_t *systab)
{
    efi_loaded_image_t *image;
    efi_status_t status;
    efi_guid_t loaded_image_proto = LOADED_IMAGE_PROTOCOL_GUID;

	WRITE_ONCE(efi_system_table, systab);

	efi_puts("Booting ArceOS ...\n");
	sbi_puts("[SBI]: Booting ArceOS ...\n");

    /*
     * Get a handle to the loaded image protocol.  This is used to get
     * information about the running image, such as size and the command
     * line.
     */
    status = efi_bs_call(handle_protocol, handle, &loaded_image_proto,
                 (void *)&image);
    if (status != EFI_SUCCESS) {
        efi_puts("Failed to get loaded image protocol\n");
        return status;
    }

#if 0
    {
        efi_puts("Image base:\n");
        efi_put_u64((u64) image->image_base);
        efi_puts("\n");
        efi_puts("Image size:\n");
        efi_put_u64((u64) image->image_size);
        efi_puts("\n");
    }
#endif

    {
        // Display _end
        u64 *p = (u64 *) (_end);
        efi_put_u64((u64) _end);
        efi_puts("\n");

        efi_put_u64(p[0]);
        efi_puts(", ");
        efi_put_u64(p[1]);
        efi_puts("\n");
    }

#if 0
    {
        efi_puts("efi_system_table:\n");
        efi_put_u64((u64) efi_system_table->boottime);
        efi_puts("\n");
        efi_put_u64((u64) efi_system_table->boottime->copy_mem);
        efi_puts("\n");
    }
#endif

    void *dst = (void *) ARCEOS_BASE;
    void *src = (void *) _end;
    size_t len = get_payload_size(image->image_base);
    efi_puts("payload size:\n");
    efi_put_u64((u64) len);
    efi_puts("\n");

    efi_bs_call(copy_mem, dst, src, len);

	efi_puts("After copy.\n");
	sbi_puts("[SBI]: After copy.\n");

#if 1
    {
        // Display 0x8020_0000
        u64 *p = (u64 *) (ARCEOS_BASE);
        efi_puts("ARCEOS_BASE(0x80200000):\n");
        efi_put_u64(p[0]);
        efi_puts(", ");
        efi_put_u64(p[1]);
        efi_puts("\n");
    }
#endif

	efi_puts("before enter.\n");
	sbi_puts("[SBI]: before enter.\n");

    {
        u64 sstatus = csr_read(0x100);
	    efi_puts("sstatus:\n");
        efi_put_u64(sstatus);
        efi_puts("\n");
    }

	//efi_enter_kernel(kernel_addr, fdt_addr, fdt_totalsize((void *)fdt_addr));
	efi_enter_kernel(ARCEOS_BASE, 0, 0);

	return 0;
}
