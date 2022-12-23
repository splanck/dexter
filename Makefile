FILES = ./build/kernel.s.o

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=10 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot.s
	nasm -f bin ./src/boot.s -o ./bin/boot.bin

./build/kernel.s.o: ./src/kernel.s
	nasm -f elf -g ./src/kernel.s -o ./build/kernel.s.o

clean:
	rm -rf ./bin/boot.bin