# SPDX-License-Identifier: GPL-2.0

PHONY := all clean

TARGET := efi_stub

OBJS := head.o entry.o printk.o

OBJS := $(addprefix src/, $(OBJS))
DEP_FILES := $(patsubst %,%.d,$(OBJS))

Q := @
CROSS_PREFIX := riscv64-linux-gnu-
CC := $(CROSS_PREFIX)gcc
LD := $(CROSS_PREFIX)ld
OBJCOPY := $(CROSS_PREFIX)objcopy
CFLAGS := -Wall -Werror -nostdinc -I. \
	-fno-PIE -march=rv64imafdc_zicsr_zifencei \
	-mno-riscv-attribute -Wa,-mno-arch-attr \
	-mabi=lp64 -mcmodel=medany -fno-stack-protector

all: $(TARGET)
	@:

$(TARGET): $(TARGET).elf
	@printf "CP\t$@\n"
	$(Q)$(OBJCOPY) $< --strip-all -O binary $@

$(TARGET).elf: $(OBJS) $(TARGET).lds
	@printf "LD\t$@\n"
	$(Q)$(LD) -nostdlib -T $(TARGET).lds -o $@ $(OBJS)

src/%.o: src/%.c
	@printf "CC\t$<\n"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

src/%.o: src/%.S
	@printf "CC\t$<\n"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

src/*.o: src/*.h src/efi-header.S

clean:
	$(Q)rm -f */*.o $(TARGET) *.elf

.PHONY: $(PHONY)
