OUTPUT_ARCH("riscv")

PECOFF_FILE_ALIGNMENT = 0x200;

ENTRY(_start)
SECTIONS
{
    _start = .;

    _stext = .;
    .text : ALIGN(4K) {
        *(.text .text.*)
    }
    . = ALIGN(4K);
    _etext = .;

    _srodata = .;
    .rodata : ALIGN(4K) {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
        *(.sdata2 .sdata2.*)
        *(.gnu*)
    }
    . = ALIGN(4K);
    _erodata = .;

    _sdata = .;
    .data : ALIGN(4K) {
        *(.data .data.*)
        *(.sdata .sdata.*)
        *(.got .got.*)
    }
	.pecoff_edata_padding : {
        BYTE(0);
        . = ALIGN(PECOFF_FILE_ALIGNMENT);
    }
    _edata = .;

    .bss : ALIGN(4K) {
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
    }

    . = ALIGN(4K);
    _end = .;

	/DISCARD/ : {
        *(.comment) *(.note*) *(.eh_frame*)
    }
}
