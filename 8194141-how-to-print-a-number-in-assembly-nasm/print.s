BITS 32
global _start

section .text
_start:

mov eax, 12345695 ; unsigned number to print
mov ebx, 36       ; base to represent the number, do not set it too big
call print

;exit
mov eax, 1
xor ebx, ebx
int 0x80

print:
mov ecx, esp
sub esp, 36   ; reserve space for the number string, for base-2 it takes 33 bytes with new line, aligned by 4 bytes it takes 36 bytes.

mov edi, 1
dec ecx
mov [ecx], byte 10 ; add a new line

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

; use system call to print, ecx is a pointer on the string
mov eax, 4    ; system call number (sys_write)
mov ebx, 1    ; file descriptor (stdout)
mov edx, edi  ; length of the string
int 0x80

add esp, 36   ; release space for the number string

ret
