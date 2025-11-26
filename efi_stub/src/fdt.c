#include "efi.h"
#include "efistub.h"

#define FDT_MAGIC   0xd00dfeed  /* 4: version, 4: total size */

#define FDT_ERR_BADMAGIC    9
    /* FDT_ERR_BADMAGIC: Given "device tree" appears not to be a
     * device tree at all - it is missing the flattened device
     * tree magic number. */

#define FDT_ERR_ALIGNMENT   19
    /* FDT_ERR_ALIGNMENT: The device tree base address is not 8-byte
     * aligned. */

#define __bitwise

typedef unsigned int __bitwise __be32;
typedef __be32 fdt32_t;

struct fdt_header {
    fdt32_t magic;           /* magic word FDT_MAGIC */
    fdt32_t totalsize;       /* total size of DT block */
    fdt32_t off_dt_struct;       /* offset to structure */
    fdt32_t off_dt_strings;      /* offset to strings */
    fdt32_t off_mem_rsvmap;      /* offset to memory reserve map */
    fdt32_t version;         /* format version */
    fdt32_t last_comp_version;   /* last compatible version */

    /* version 2 fields below */
    fdt32_t boot_cpuid_phys;     /* Which physical CPU id we're
                        booting on */
    /* version 3 fields below */
    fdt32_t size_dt_strings;     /* size of the strings block */

    /* version 17 fields below */
    fdt32_t size_dt_struct;      /* size of the structure block */
};

/**********************************************************************/
/* General functions                                                  */
/**********************************************************************/
#define fdt_get_header(fdt, field) \
    (fdt32_ld(&((const struct fdt_header *)(fdt))->field))
#define fdt_magic(fdt)          (fdt_get_header(fdt, magic))
#define fdt_totalsize(fdt)      (fdt_get_header(fdt, totalsize))

static inline unsigned int fdt32_ld(const fdt32_t *p)
{
    const unsigned char *bp = (const unsigned char *)p;

    return ((unsigned int)bp[0] << 24)
        | ((unsigned int)bp[1] << 16)
        | ((unsigned int)bp[2] << 8)
        | bp[3];
}

/**
 * get_efi_config_table() - retrieve UEFI configuration table
 * @guid:   GUID of the configuration table to be retrieved
 * Return:  pointer to the configuration table or NULL
 */
static void *get_efi_config_table(efi_guid_t guid)
{
    unsigned long tables = efi_table_attr(efi_system_table, tables);
    int nr_tables = efi_table_attr(efi_system_table, nr_tables);
    int i;

    for (i = 0; i < nr_tables; i++) {
        efi_config_table_t *t = (void *)tables;

        if (efi_guidcmp(t->guid, guid) == 0)
            return efi_table_attr(t, table);

        tables += sizeof(efi_config_table_t);
    }
    return NULL;
}

int fdt_check_header(const void *fdt)
{
    /* The device tree must be at an 8-byte aligned address */
    if ((unsigned long)fdt & 7)
        return -FDT_ERR_ALIGNMENT;

    if (fdt_magic(fdt) != FDT_MAGIC)
        return -FDT_ERR_BADMAGIC;

    return 0;
}

void *get_fdt(unsigned long *fdt_size)
{
    void *fdt;

    fdt = get_efi_config_table(DEVICE_TREE_GUID);

    if (!fdt)
        return NULL;

    if (fdt_check_header(fdt) != 0) {
        efi_puts("Invalid header detected on UEFI supplied FDT, ignoring ...\n");
        return NULL;
    }
    *fdt_size = fdt_totalsize(fdt);
    return fdt;
}
