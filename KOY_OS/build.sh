#!/bin/bash

set -e

echo "======================================"
echo "        Building KOY OS"
echo "======================================"

# Clean old files
rm -f bin/boot.bin
rm -f bin/obj/kernel.o
rm -f bin/obj/vga.o
rm -f bin/obj/koy_string.o
rm -f bin/obj/kernel_entry.o
rm -f disp/kernel.elf
rm -f bin/kernel.bin
rm -f dist/koy.img

echo ""
echo "[1/9] Assembling bootloader..."

nasm -f bin source/boot/boot.asm \
    -o bin/boot.bin

echo "Bootloader built."

echo ""
echo "[2/9] Checking bootloader size..."

BOOT_SIZE=$(stat -c%s bin/boot.bin)

if [ "$BOOT_SIZE" -ne 512 ]; then
    echo "ERROR: boot.bin must be exactly 512 bytes."
    echo "Current size: $BOOT_SIZE bytes"
    exit 1
fi

echo "boot.bin = 512 bytes"

echo ""
echo "[3/9] Assembling kernel entry..."

nasm -f elf32 source/boot/kernel_entry.asm \
    -o bin/obj/kernel_entry.o

echo "kernel_entry.o built."

echo ""
echo "[4/9] Compiling KOY string library..."

gcc -m32 \
    -ffreestanding \
    -fno-pie \
    -fno-stack-protector \
    -Iinclude \
    -c source/include/koy_string.c \
    -o bin/obj/koy_string.o

echo "koy_string.o built."

echo ""
echo "[5/9] Compiling C kernel..."

gcc -m32 \
    -ffreestanding \
    -fno-pie \
    -fno-stack-protector \
    -Iinclude \
    -c source/kernel.c \
    -o bin/obj/kernel.o

echo "kernel.o built."

echo ""
echo "[6/9] Compiling VGA driver..."

gcc -m32 \
    -ffreestanding \
    -fno-pie \
    -fno-stack-protector \
    -Iinclude \
    -c source/include/vga.c \
    -o bin/obj/vga.o

echo "vga.o built."

echo ""
echo "[7/9] Linking kernel..."

ld -m elf_i386 \
    -T source/linker.ld \
    -o disp/kernel.elf \
    bin/obj/kernel_entry.o \
    bin/obj/kernel.o \
    bin/obj/vga.o \
    bin/obj/koy_string.o

echo "kernel.elf built."

echo ""
echo "[8/9] Converting ELF kernel to raw binary..."

objcopy -O binary \
    disp/kernel.elf \
    bin/kernel.bin

echo "kernel.bin built."

echo ""
echo "[9/9] Creating KOY OS disk image..."

# Create 1.44 MB floppy image
dd if=/dev/zero \
    of=dist/koy.img \
    bs=512 \
    count=2880 \
    status=none

# Sector 1 = bootloader
dd if=bin/boot.bin \
    of=dist/koy.img \
    bs=512 \
    seek=0 \
    conv=notrunc \
    status=none

# Sector 2 onward = kernel
dd if=bin/kernel.bin \
    of=dist/koy.img \
    bs=512 \
    seek=1 \
    conv=notrunc \
    status=none

echo ""
echo "======================================"
echo "        KOY OS BUILD SUCCESSFUL"
echo "======================================"

echo ""
echo "Files generated:"
echo ""

ls -lh bin/boot.bin
ls -lh bin/obj/kernel_entry.o
ls -lh bin/obj/koy_string.o
ls -lh bin/obj/kernel.o
ls -lh bin/obj/vga.o
ls -lh disp/kernel.elf
ls -lh bin/kernel.bin
ls -lh dist/koy.img

echo ""
echo "======================================"
echo "Run KOY OS with:"
echo ""
echo "qemu-system-x86_64 -drive format=raw,file=dist/koy.img"
echo "======================================"

qemu-system-x86_64 \
    -drive format=raw,file=dist/koy.img