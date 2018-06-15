bits 32
global _start

section .data
	rows dw 10

section .text
_start:
movzx ebx, word [rows] ; ebx holds number of rows, used in loop

; here we count how many symbols we have
lea eax, [ebx+3]
imul eax,ebx
shr eax,1 ; now eax holds number of all symbols
mov edx, eax ; now edx holds number of all symbols, used in print

;we prepare stack to fill data
mov ecx,esp
sub esp,edx

;we fill stack backwards
next_line:
	dec ecx 
	mov [ecx],byte 10
	mov eax,ebx
	next_star:
		dec ecx
		mov [ecx],byte '*'
		dec eax
		jg next_star
	dec ebx
	jg next_line

;print ; edx has number of char; ecx is pointer on the string
mov eax,4;	;sys_write
inc ebx;	;1 - stdout, at the end of the loop we have ebx=0
int 80h;

;exit
mov eax,1		;1 -  sys_exit
xor ebx,ebx		;0 - return 0
int 80h;
ret
