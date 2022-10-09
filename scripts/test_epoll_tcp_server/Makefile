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
	$(CC) $(CFLAGS) $(INC) $(SRCS) -o _install/main -static -lpthread
	cd _install && find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../busybox.o
	qemu-system-x86_64 -kernel ../../arch/x86/boot/bzImage -nographic -initrd busybox.o --append "console=ttyS0 nokaslr root=/dev/ram" -nographic -s -S

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) init rootfs.o