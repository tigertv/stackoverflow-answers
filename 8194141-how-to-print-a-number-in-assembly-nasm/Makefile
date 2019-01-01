.PHONY: clean all

AS = nasm
LNK = ld
SRC = $(wildcard *.asm)
BIN := $(SRC:.asm=)

all: $(BIN)
	
%: %.o
	$(LNK) -melf_i386 $< -o $@
%.o: %.asm
	$(AS) -f elf $< -o $@
clean:
	rm -f $(BIN)
