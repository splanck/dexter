all:
    mkdir ./build
    nasm -f elf64 ./blank.asm -o ./build/blank.o
    x86_64-elf-gcc -g -T ../../linker64.ld -o blank.bin -ffreestanding -O0 -nostdlib -fpic -g ./build/blank.o

clean:
	rm -rf ./build/
	
