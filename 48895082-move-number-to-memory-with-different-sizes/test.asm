global _start

section .data
    var dw 0x10, 0x20, 0x30

section .text
_start:

mov edx, dword 0x12345678
mov [var], edx

; quit the program
mov eax, 1		; sys_exit system call
push dword 0		; program return value
push eax
int 0x80		; call the kernel
