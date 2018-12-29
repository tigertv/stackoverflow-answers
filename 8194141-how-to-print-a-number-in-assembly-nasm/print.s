BITS 32
global _start

section .text
_start:

mov eax, 123456
call print

;exit
mov eax, 1
xor ebx, ebx
int 0x80

print:
mov edi, 1
mov ecx, esp
mov ebx, 10
dec ecx
mov [ecx], bl

print_loop:

xor edx, edx
idiv ebx
add dl, '0'
dec ecx
inc edi
mov [ecx],dl
test eax, eax
jnz print_loop

mov eax, 4
mov ebx, 1
mov edx, edi
int 0x80

ret
