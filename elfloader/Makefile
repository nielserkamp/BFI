SHELL=bash
CC=gcc
AS=gcc
LINK=gcc
STRIP=strip --strip-all

ARGV=/bin/ls

PAYLOAD_LDFLAGS=-z noexecstack -nostdlib 

SETTINGS=-Os -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fPIE
WARNINGS=-Wno-unused-parameter -pedantic -std=gnu99

CFLAGS=-fno-stack-protector $(WARNINGS) $(SETTINGS)

TARGETS=elfloader elfpayload try_payload

all: $(TARGETS)

strip:
	$(STRIP) elfloader

clean:
	-$(RM) elfloader.o syscalls.o $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

syscalls.o: syscalls.S
	$(AS) -c -o $@ $<

elfloader: elfloader.ld elfloader.o syscalls.o
	$(LINK) $(PAYLOAD_LDFLAGS) -o $@ -T elfloader.ld elfloader.o syscalls.o
	echo -n $$'\x03'|dd of=$@ bs=1 seek=16 conv=notrunc
	$(STRIP) $@

try_payload: try_payload.c
	$(LINK) $(LDFLAGS) -o $@ $<

elfpayload: elfloader mkinjector.py
	python mkinjector.py $(ARGV) > $@
