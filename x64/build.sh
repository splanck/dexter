rm -rf ./build/os.bin

nasm -f bin -o ./build/boot.bin ./src/boot.asm
nasm -f bin -o ./build/loader.bin ./src/loader.asm

dd if=./build/boot.bin >> ./build/os.bin
dd if=./build/loader.bin >> ./build/os.bin
dd if=/dev/zero bs=1048576 count=5 >> ./build/os.bin

qemu-system-x86_64 -hda ./build/os.bin

