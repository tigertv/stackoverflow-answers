.PHONY: clean all
ASM=nasm
SRC = $(wildcard *.asm) 
OBJ = $(SRC:.asm=.o)
OUT = $(SRC:.asm=.out)

all: $(OUT) 

%.o: %.asm
	$(ASM) -f elf $< 
%.out: %.o
	ld -o $@ $< 

clean: 
	rm -rf $(OBJ) $(OUT)
