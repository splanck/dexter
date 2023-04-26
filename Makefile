C_COMPILER = i686-elf-gcc
FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt.asm.o ./build/idt.o ./build/memory.o ./build/io.asm.o ./build/paging.asm.o ./build/console.o ./build/string.o ./build/heap.o ./build/kheap.o ./build/paging.o ./build/disk.o ./build/pathparser.o ./build/streamer.o ./build/file.o ./build/fat16.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=5 >> ./bin/os.bin
	sudo mkdir -p /mnt/d
	sudo mount -t vfat ./bin/os.bin /mnt/d
	sudo cp ./LICENSE /mnt/d
	sudo umount /mnt/d
	cp ./bin/os.bin ./images/dexter.img

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/sys/boot.asm
	nasm -f bin ./src/sys/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/sys/kernel.asm
	nasm -f elf -g ./src/sys/kernel.asm -o ./build/kernel.asm.o

./build/idt.asm.o: ./src/sys/idt.asm
	nasm -f elf -g ./src/sys/idt.asm -o ./build/idt.asm.o

./build/io.asm.o: ./src/sys/io.asm
	nasm -f elf -g ./src/sys/io.asm -o ./build/io.asm.o

./build/paging.asm.o: ./src/mem/paging.asm
	nasm -f elf -g ./src/mem/paging.asm -o ./build/paging.asm.o

./build/kernel.o: ./src/sys/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/sys/kernel.c -o ./build/kernel.o
	
./build/console.o: ./src/lib/console.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/lib/console.c -o ./build/console.o

./build/fat16.o: ./src/fs/fat16.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/fat16.c -o ./build/fat16.o

./build/file.o: ./src/fs/file.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/file.c -o ./build/file.o

./build/streamer.o: ./src/fs/streamer.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/streamer.c -o ./build/streamer.o

./build/pathparser.o: ./src/fs/pathparser.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/pathparser.c -o ./build/pathparser.o

./build/disk.o: ./src/fs/disk.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/disk.c -o ./build/disk.o

./build/idt.o: ./src/sys/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/sys/idt.c -o ./build/idt.o

./build/string.o: ./src/lib/string.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/lib/string.c -o ./build/string.o

./build/paging.o: ./src/mem/paging.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/paging.c -o ./build/paging.o

./build/kheap.o: ./src/mem/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/kheap.c -o ./build/kheap.o

./build/heap.o: ./src/mem/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/heap.c -o ./build/heap.o

./build/memory.o: ./src/mem/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/memory.c -o ./build/memory.o

run:
	qemu-system-i386 -hda ./images/dexter.img

clean:
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ./build