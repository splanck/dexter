#!/bin/bash
nasm -f bin -o boot.bin ./src/boot.asm
nasm -f bin -o loader.bin ./src/loader.asm
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
dd if=loader.bin of=boot.img bs=512 count=5 seek=1 conv=notrunc
