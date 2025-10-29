# SPDX-License-Identifier: GPL-2.0

PHONY := all clean

TARGET := mk_img

OBJS := mk_img.o util.o

Q := @
CC := gcc
CFLAGS := -Wall -Werror

all: $(TARGET)

$(TARGET): $(OBJS)
	@printf "LD\t$@\n"
	$(Q)$(CC) -o $@ $^

%.o: %.c
	@printf "CC\t$<\n"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

*.o: *.h

clean:
	$(Q)rm -f *.o $(TARGET)

.PHONY: $(PHONY)
