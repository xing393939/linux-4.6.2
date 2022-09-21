CC = gcc
CFLAGS = -g -O0 -W  -Wall

COMMON_PATH = ../test_common
INC = -I . -I $(COMMON_PATH)

SRCS = $(wildcard ./*.c) $(wildcard $(COMMON_PATH)/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

CURRENT_DIR = $(notdir $(shell pwd))
TARGET = $(CURRENT_DIR)

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

rootfs:
	$(CC) $(CFLAGS) $(INC) $(SRCS) -o init -static -lpthread
	find init | cpio -o -Hnewc | gzip -9 > rootfs.o
	qemu-system-x86_64 -kernel ../../arch/x86/boot/bzImage -nographic -initrd rootfs.o --append "console=ttyS0 nokaslr root=/dev/ram" -nographic

busybox:
	qemu-system-x86_64 -kernel ../../arch/x86/boot/bzImage -nographic -initrd busybox.img --append "console=ttyS0 nokaslr root=/dev/ram" -nographic

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) init rootfs.o