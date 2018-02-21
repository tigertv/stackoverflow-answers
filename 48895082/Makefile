.PHONY: clean all
ASM=nasm

all: test

test.o: test.asm
	$(ASM) -f elf test.asm 
test: test.o
	ld -o test test.o
clean: 
	rm -rf test *.o 
