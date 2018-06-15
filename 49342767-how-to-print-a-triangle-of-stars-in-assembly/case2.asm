global _start

section .data
	rows dw 10
;	stars dq 'A***','*A**','**A*','***A','****'

section .text
_start:

;it defines how many symbols we have to print
movzx ebx, byte[rows] ; ebx holds number of rows
lea eax,[ebx+3]
imul eax,ebx 
shr eax,1 ; now eax holds number of all symbols
mov edx,eax ; now edx holds number of all symbols, used in print

;prepare pointer
mov ecx,esp
sub ecx,eax ; ecx points on the beginning of the string, used in print

;fill the string by stars
mov eax,edx
shr eax,2
mov ebp, dword '****'
next_star:
	mov [ecx+4*eax],ebp
	dec eax
	jge next_star

;fill the string by '\n'
mov edi,esp
dec edi
mov eax,ebx; in the eax is number of rows
inc eax
next_n:
	mov [edi],byte 0xa
	sub edi,eax
	dec eax
	jg next_n

;print
;mov ecx,esp
mov eax,4;	;sys_write
mov ebx,1;	;1 - stdout 
int 80h;

;exit
mov eax,1		;1 -  sys_exit
xor ebx,ebx		;0 - return 0
int 80h;
ret
