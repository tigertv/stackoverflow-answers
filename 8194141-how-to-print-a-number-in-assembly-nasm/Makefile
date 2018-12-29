
.PHONY: clean 

print: print.o
	ld -melf_i386 $< -o $@
print.o: print.s
	nasm -f elf $< -o $@

clean:
	rm -f print.o print 

