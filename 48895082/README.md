# [Question]()
I am learning Assembly and I found something I don't understand clearly.

If I have code like this
```
section .data
    var dw 0x10, 0x20, 0x30

section .text
; ...
mov edx, dword 0x12345678
mov [var], edx
; ...
```
I do know already, that result will be <br>

[var] = 0x5678<br>
[var + 2] = 0x1234<br>
I just don't know what happened inside.<br>

Why was dx = 0x5678 stored as the first element and not the second? And after this half is stored, does the first half of edx suddenly becomes word instead of double word?<br>

How it works at all, if I attempt to put a bigger number into memory than it is reserved (let's say 256 into var_b resb 1)?<br>

# [Answer]()

Your code stores 0x12345678 in edx, and then copy the value from edx in place where the var is placed. Lowest byte(8 bit) goes first when they are storied in memory from a register, because x86 processors are little endian.<br>

Code compiled by nasm:<br>

```asm
global _start

section .data
    var dw 0x10, 0x20, 0x30

	section .text
	_start:

	mov edx, dword 0x12345678
	mov [var], edx

	; quit the program
	mov eax, 1      ; sys_exit system call
	push dword 0        ; program return value
	push eax
	int 0x80        ; call the kernel
```
Debugger:<br>
```
(gdb) b _start 
Breakpoint 1 at 0x8048080
(gdb) r
Starting program: /home/ssssssssssssssss/test 

Breakpoint 1, 0x08048080 in _start ()
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>: mov    edx,0x12345678
0x08048085 <+5>: mov    DWORD PTR ds:0x804908c,edx
End of assembler dump.
(gdb) x/4x 0x804908c
0x804908c:  0x10 0x0 0x20 0x0
(gdb) si
0x08048085 in _start ()
(gdb) disassemble 
Dump of assembler code for function _start:
0x08048080 <+0>: mov    edx,0x12345678
=> 0x08048085 <+5>: mov    DWORD PTR ds:0x804908c,edx
End of assembler dump.
(gdb) i r
eax            0x0  0
ecx            0x0  0
edx            0x12345678   305419896
ebx            0x0  0
esp            0xbfffee70   0xbfffee70
ebp            0x0  0x0
esi            0x0  0
edi            0x0  0
eip            0x8048085    0x8048085 <_start+5>
eflags         0x202    [ IF ]
cs             0x73 115
ss             0x7b 123
ds             0x7b 123
es             0x7b 123
fs             0x0  0
gs             0x0  0
(gdb) si
0x0804808b in ?? ()
(gdb) x/4x 0x804908c
0x804908c:  0x78    0x56    0x34    0x12
(gdb) 
```
objdump:
```

test:     file format elf32-i386


Disassembly of section .text:

08048080 <_start>:
8048080:       ba 78 56 34 12          mov    edx,0x12345678
8048085:       89 15 8c 90 04 08       mov    DWORD PTR ds:0x804908c,edx

```
