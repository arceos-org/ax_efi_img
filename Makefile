# SPDX-License-Identifier: GPL-2.0

PHONY := all clean

Q := @

all:
	$(Q)make -C efi_stub
	$(Q)make -C mk_img

clean:
	$(Q)make -C efi_stub clean
	$(Q)make -C mk_img clean

.PHONY: $(PHONY)
