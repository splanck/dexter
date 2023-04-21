C_COMPILER = i686-elf-gcc
FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt.asm.o ./build/idt.o ./build/memory.o ./build/io.asm.o ./build/paging.asm.o ./build/console.o ./build/string.o ./build/heap.o ./build/kheap.o ./build/paging.o ./build/disk.o ./build/pathparser.o ./build/streamer.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot.asm
	nasm -f bin ./src/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/idt.asm.o: ./src/idt.asm
	nasm -f elf -g ./src/idt.asm -o ./build/idt.asm.o

./build/io.asm.o: ./src/io.asm
	nasm -f elf -g ./src/io.asm -o ./build/io.asm.o

./build/paging.asm.o: ./src/paging.asm
	nasm -f elf -g ./src/paging.asm -o ./build/paging.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o
	
./build/console.o: ./src/console.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/console.c -o ./build/console.o

./build/streamer.o: ./src/streamer.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/streamer.c -o ./build/streamer.o

./build/pathparser.o: ./src/pathparser.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/pathparser.c -o ./build/pathparser.o

./build/disk.o: ./src/disk.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/disk.c -o ./build/disk.o

./build/idt.o: ./src/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/idt.c -o ./build/idt.o

./build/string.o: ./src/string.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/string.c -o ./build/string.o

./build/paging.o: ./src/paging.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/paging.c -o ./build/paging.o

./build/kheap.o: ./src/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kheap.c -o ./build/kheap.o

./build/heap.o: ./src/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/heap.c -o ./build/heap.o

./build/memory.o: ./src/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory.c -o ./build/memory.o

clean:
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ./build