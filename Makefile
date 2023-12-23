C_COMPILER = i686-elf-gcc
FILES = ./build/kernel.asm.o ./build/idt.asm.o ./build/io.asm.o ./build/task.asm.o ./build/tss.asm.o ./build/gdt.asm.o ./build/paging.asm.o ./build/kernel.o ./build/idt.o ./build/memory.o ./build/stdio.o ./build/string.o ./build/heap.o ./build/kheap.o ./build/paging.o ./build/disk.o ./build/pparser.o ./build/streamer.o ./build/file.o ./build/fat16.o ./build/gdt.o ./build/task.o ./build/process.o ./build/isr80h.o ./build/commands.o ./build/keyboard.o ./build/classic.o ./build/math.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin user_programs
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=5 >> ./bin/os.bin
	sudo mkdir -p /mnt/d
	sudo mount -t vfat ./bin/os.bin /mnt/d
	sudo cp ./doc/hello.txt /mnt/d
	sudo cp ./progs/blank/blank.bin /mnt/d
	sudo cp ./LICENSE /mnt/d/license.txt
	sudo umount /mnt/d
	cp ./bin/os.bin ./images/dexter.img

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/x86/boot.asm
	nasm -f bin ./src/x86/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/x86/kernel.asm
	nasm -f elf -g ./src/x86/kernel.asm -o ./build/kernel.asm.o

./build/gdt.asm.o: ./src/x86/gdt.asm
	nasm -f elf -g ./src/x86/gdt.asm -o ./build/gdt.asm.o

./build/idt.asm.o: ./src/x86/idt.asm
	nasm -f elf -g ./src/x86/idt.asm -o ./build/idt.asm.o

./build/io.asm.o: ./src/x86/io.asm
	nasm -f elf -g ./src/x86/io.asm -o ./build/io.asm.o

./build/task.asm.o: ./src/x86/task.asm
	nasm -f elf -g ./src/x86/task.asm -o ./build/task.asm.o

./build/tss.asm.o: ./src/x86/tss.asm
	nasm -f elf -g ./src/x86/tss.asm -o ./build/tss.asm.o

./build/paging.asm.o: ./src/x86/paging.asm
	nasm -f elf -g ./src/x86/paging.asm -o ./build/paging.asm.o

./build/kernel.o: ./src/sys/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/sys/kernel.c -o ./build/kernel.o
	
./build/gdt.o: ./src/sys/gdt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/sys/gdt.c -o ./build/gdt.o

./build/keyboard.o: ./src/dev/keyboard.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/dev/keyboard.c -o ./build/keyboard.o

./build/classic.o: ./src/dev/classic.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/dev/classic.c -o ./build/classic.o

./build/fat16.o: ./src/fs/fat16.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/fat16.c -o ./build/fat16.o

./build/file.o: ./src/fs/file.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/file.c -o ./build/file.o

./build/streamer.o: ./src/fs/streamer.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/streamer.c -o ./build/streamer.o

./build/pparser.o: ./src/fs/pparser.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/pparser.c -o ./build/pparser.o

./build/disk.o: ./src/fs/disk.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/disk.c -o ./build/disk.o

./build/idt.o: ./src/sys/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/sys/idt.c -o ./build/idt.o

./build/paging.o: ./src/mem/paging.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/paging.c -o ./build/paging.o

./build/kheap.o: ./src/mem/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/kheap.c -o ./build/kheap.o

./build/heap.o: ./src/mem/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/heap.c -o ./build/heap.o

./build/memory.o: ./src/mem/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/mem/memory.c -o ./build/memory.o

./build/task.o: ./src/proc/task.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/proc/task.c -o ./build/task.o

./build/process.o: ./src/proc/process.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/proc/process.c -o ./build/process.o

./build/isr80h.o: ./src/proc/isr80h.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/proc/isr80h.c -o ./build/isr80h.o

./build/commands.o: ./src/proc/commands.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/proc/commands.c -o ./build/commands.o

./build/math.o: ./src/libc/math.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/libc/math.c -o ./build/math.o

./build/string.o: ./src/libc/string.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/libc/string.c -o ./build/string.o

./build/stdio.o: ./src/libc/stdio.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/libc/stdio.c -o ./build/stdio.o

user_programs:
	cd ./progs/blank && $(MAKE) all

user_programs_clean:
	cd ./progs/blank && $(MAKE) clean

run:
	qemu-system-i386 -hda ./images/dexter.img

clean: user_programs_clean
	rm -rf ./bin
	rm -rf ${FILES}
	rm -rf ./buildt