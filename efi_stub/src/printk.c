// SPDX-License-Identifier: GPL-2.0

#include "efistub.h"

#define UL_STR_SIZE 19  /* prefix with '0x' and end with '\0' */

static
u32 utf8_to_utf32(const u8 **s8)
{
	u32 c32;
	u8 c0, cx;
	size_t clen, i;

	c0 = cx = *(*s8)++;
	/*
	 * The position of the most-significant 0 bit gives us the length of
	 * a multi-octet encoding.
	 */
	for (clen = 0; cx & 0x80; ++clen)
		cx <<= 1;
	/*
	 * If the 0 bit is in position 8, this is a valid single-octet
	 * encoding. If the 0 bit is in position 7 or positions 1-3, the
	 * encoding is invalid.
	 * In either case, we just return the first octet.
	 */
	if (clen < 2 || clen > 4)
		return c0;
	/* Get the bits from the first octet. */
	c32 = cx >> clen--;
	for (i = 0; i < clen; ++i) {
		/* Trailing octets must have 10 in most significant bits. */
		cx = (*s8)[i] ^ 0x80;
		if (cx & 0xc0)
			return c0;
		c32 = (c32 << 6) | cx;
	}
	/*
	 * Check for validity:
	 * - The character must be in the Unicode range.
	 * - It must not be a surrogate.
	 * - It must be encoded using the correct number of octets.
	 */
	if (c32 > 0x10ffff ||
	    (c32 & 0xf800) == 0xd800 ||
	    clen != (c32 >= 0x80) + (c32 >= 0x800) + (c32 >= 0x10000))
		return c0;
	*s8 += clen;
	return c32;
}

/**
 * efi_char16_puts() - Write a UCS-2 encoded string to the console
 * @str:	UCS-2 encoded string
 */
void efi_char16_puts(efi_char16_t *str)
{
	efi_call_proto(efi_table_attr(efi_system_table, con_out),
		       output_string, str);
}

/**
 * efi_puts() - Write a UTF-8 encoded string to the console
 * @str:	UTF-8 encoded string
 */
void efi_puts(const char *str)
{
	efi_char16_t buf[128];
	size_t pos = 0, lim = ARRAY_SIZE(buf);
	const u8 *s8 = (const u8 *)str;
	u32 c32;

	while (*s8) {
		if (*s8 == '\n')
			buf[pos++] = L'\r';
		c32 = utf8_to_utf32(&s8);
		if (c32 < 0x10000) {
			/* Characters in plane 0 use a single word. */
			buf[pos++] = c32;
		} else {
			/*
			 * Characters in other planes encode into a surrogate
			 * pair.
			 */
			buf[pos++] = (0xd800 - (0x10000 >> 10)) + (c32 >> 10);
			buf[pos++] = 0xdc00 + (c32 & 0x3ff);
		}
		if (*s8 == '\0' || pos >= lim - 2) {
			buf[pos] = L'\0';
			efi_char16_puts(buf);
			pos = 0;
		}
	}
}

static void put_char(u8 ch)
{
	__asm__ __volatile__ (
        "li a7, 1\n"
        "mv a0, %0\n"
        "ecall\n"
    : : "r" (ch) : "memory");
}

/**
 * efi_puts() - Write a UTF-8 encoded string to the console
 * @str:	UTF-8 encoded string
 */
void sbi_puts(const char *str)
{
	const u8 *s8 = (const u8 *)str;

	while (*s8) {
        put_char(*s8);
        s8++;
	}
}

static int hex_to_str(unsigned long n, char *str, size_t len)
{
    /* prefix with '0x' and end with '\0' */
    if (len != 19)
        return -1;

    str[0] = '0';
    str[1] = 'x';
    for (int i = 3; i <= 18; i++) {
        char c = (n >> ((18 - i)*4)) & 0xF;
        if (c >= 10) {
            c -= 10;
            c += 'A';
        } else {
            c += '0';
        }
        str[i-1] = c;
    }
    str[18] = '\0';

    return 0;
}

void efi_put_u64(unsigned long n)
{
    char buf[UL_STR_SIZE];
    hex_to_str(n, buf, sizeof(buf));
    efi_puts(buf);
}
