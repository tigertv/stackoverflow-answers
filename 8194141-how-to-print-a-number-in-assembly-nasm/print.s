BITS 32
global _start

section .text
_start:

mov eax, 123 ; unsigned number to print
mov ebx, 12  ; base to represent the number, do not set it too big
call print

;exit
mov eax, 1
xor ebx, ebx
int 0x80

print:
mov edi, 1
mov ecx, esp
add esp, 32
dec ecx
mov edx, 10
mov [ecx], dl

print_loop:

xor edx, edx
div ebx
cmp dl, 9     ; if reminder>9 go to use_letter
jg use_letter

add dl, '0'
jmp after_use_letter

use_letter:
add dl, 'W'   ; letters from 'a' to ... in ascii code

after_use_letter:
dec ecx
inc edi
mov [ecx],dl
test eax, eax
jnz print_loop

sub esp, 32

mov eax, 4
mov ebx, 1
mov edx, edi
int 0x80

ret
