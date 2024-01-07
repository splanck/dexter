nasm -f bin -o ./build/boot.bin ./src/boot.asm

dd if=./build/boot.bin of=./images/dexter64.img bs=512 count=1 conv=notrunc

qemu-system-x86_64 -hda ./images/dexter64.img