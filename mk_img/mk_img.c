#include <stdlib.h>
#include "util.h"

#define EFI_MAGIC 0x5a4d

struct context
{
    uint32_t    stub_file_size;
    uint32_t    stub_mem_size;
    uint32_t    pecoff_size;
    uint32_t    payload_size;
    FILE        *fp;            /* final efi image file pointer */
};

static int
cat_file(FILE *fp_dst, FILE *fp_src)
{
    int ret;
    char buf[16];

    while (1) {
        ret = fread(buf, 1, sizeof(buf), fp_src);
        if (ret == 0) {
            break;
        }
        BUG_ON(ret < 0, "read file");

        if (fwrite(buf, 1, ret, fp_dst) != ret) {
            PANIC("write file");
        }
    }
    return 0;
}

static int
make_efi_img(struct context *ctx,
             const char *efi_img,
             const char *efi_stub,
             const char *payload)
{
    FILE *fp_stub;
    FILE *fp_payload;

    BUG_ON(ctx == NULL, "No context");

    fp_stub = fopen(efi_stub, "rb");
    BUG_ON(fp_stub == NULL, "no efi stub file");
    fseek(fp_stub, 0, SEEK_END);
    ctx->stub_file_size = ftell(fp_stub);
    fseek(fp_stub, 0, SEEK_SET);

    fp_payload = fopen(payload, "rb");
    BUG_ON(fp_payload == NULL, "no panload file");
    fseek(fp_payload, 0, SEEK_END);
    ctx->payload_size = ftell(fp_payload);
    fseek(fp_payload, 0, SEEK_SET);

    ctx->fp = fopen(efi_img, "w+b");
    BUG_ON(ctx->fp == NULL, "make efi img error");

    cat_file(ctx->fp, fp_stub);
    cat_file(ctx->fp, fp_payload);

    fclose(fp_stub);
    fclose(fp_payload);

    /* Reset fp to start position */
    fseek(ctx->fp, 0, SEEK_SET);
    return 0;
}

static int check_efi_stub_header(struct context *ctx)
{
    uint32_t payload_size_1;
    uint32_t payload_size_2;

    BUG_ON(read_word(ctx->fp, 0) != EFI_MAGIC, "bad MAGIC of efi image");

    /* The initial EFI_IMG_SIZE is the size of efi stub itself. */
    ctx->stub_mem_size = read_dword(ctx->fp, EFI_IMG_SIZE_POS);
    BUG_ON(ctx->stub_mem_size == 0, "bad efi stub, mem size is ZERO");
    printf("efi stub mem size: 0x%x\n", ctx->stub_mem_size);

    /* The initial size in pecoff header is just as EFI_IMG_SIZE. */
    ctx->pecoff_size = read_dword(ctx->fp, PECOFF_SIZE_POS);
    BUG_ON(ctx->pecoff_size != ctx->stub_mem_size,
           "pecoff_header differs from stub_header in SIZE");

    /* The initial size of payload in section 'load' should be Zero. */
    payload_size_1 = read_dword(ctx->fp, PAYLOAD_SIZE_1_POS);
    payload_size_2 = read_dword(ctx->fp, PAYLOAD_SIZE_2_POS);
    BUG_ON(payload_size_1 != 0 || payload_size_2 != 0,
           "payload size must be ZERO");

    return 0;
}

static int
fixup_efi_img_header(struct context *ctx)
{
    int ret;
    uint32_t efi_img_mem_size;

    efi_img_mem_size = ctx->stub_mem_size + ctx->payload_size;

    /* [EFI_IMG_SIZE_POS] += Payload_Size */
    ret = write_dword(ctx->fp, EFI_IMG_SIZE_POS, efi_img_mem_size);
    BUG_ON(ret, "fixup [EFI_IMG_SIZE_POS] error");

    /* [PECOFF_SIZE_POS] += Payload_Size */
    ret = write_dword(ctx->fp, PECOFF_SIZE_POS, efi_img_mem_size);
    BUG_ON(ret, "fixup [PECOFF_SIZE_POS] error");

    /* [PAYLOAD_SIZE_1_POS] = Payload_Size */
    ret = write_dword(ctx->fp, PAYLOAD_SIZE_1_POS, ctx->payload_size);
    BUG_ON(ret, "fixup [PAYLOAD_SIZE_1_POS] error");

    /* [PAYLOAD_SIZE_2_POS] = Payload_Size */
    ret = write_dword(ctx->fp, PAYLOAD_SIZE_2_POS, ctx->payload_size);
    BUG_ON(ret, "fixup [PAYLOAD_SIZE_2_POS] error");

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    struct context ctx = {};
    const char *EFI_IMG;
    const char *EFI_STUB;
    const char *PAYLOAD;

    BUG_ON(argc != 4, "Usage: mk_img [EFI_IMG] [EFI_STUB_TMPL] [PAYLOAD]");
    EFI_IMG  = argv[1];
    EFI_STUB = argv[2];
    PAYLOAD  = argv[3];

    printf("make '%s' based on '%s' and '%s'.\n",
           EFI_IMG, EFI_STUB, PAYLOAD);

    ret = make_efi_img(&ctx, EFI_IMG, EFI_STUB, PAYLOAD);
    BUG_ON(ret, "Make efi img error");
    printf("efi stub file size: 0x%x\n", ctx.stub_file_size);
    printf("payload file size: 0x%x\n", ctx.payload_size);

    /* Check header which comes from efi stub. */
    ret = check_efi_stub_header(&ctx);
    BUG_ON(ret, "Bad efi image header");

    /* Fixup efi image header. */
    ret = fixup_efi_img_header(&ctx);
    BUG_ON(ret, "Make efi image header");

    fclose(ctx.fp);
    printf("make '%s' ok!\n", EFI_IMG);
    return 0;
}
