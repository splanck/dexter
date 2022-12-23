FILES = ./build/kernel.s.o

all: ./bin/boot.bin $(FILES)
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin

./build/boot.bim: ./stc/boot.s
	nasm -f bin ./src/boot.s -o ./bin/boot.bin

./build/kernel.s.o: ./src/kernel.s
	nasm -f elf -g ./src/kernel.s -o ./bin/kernel.s.o

clean:
	rm -rf ./bin/boot.bin