All:
	nasm -f bin ./src/boot.s -o ./bin/boot.bin

clean:
	rm -rf ./bin/boot.bin