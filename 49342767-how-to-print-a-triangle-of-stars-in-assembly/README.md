# [Question](https://stackoverflow.com/questions/49342767/how-to-print-a-triangle-of-stars-in-assembly)

 need to get the following output:

```
*
**
***
****
*****
******
*******
********
*********
**********
```
So its 10 rows,and my stars will start at 1 and go to 10.

Currently I am getting:

```
**********
***********
************
*************
**************
***************
****************
*****************
******************
*******************
********************
```
My code:

```asm
section .data

char db ' '
trianglesize db 0;      ;stars per line
trianglerows db 10;

section .text
global _start
_start

mov rax, [trianglerows] ;rows
outer_loop:
    mov rbx, [trianglerows]
    inner_loop:
    call star
    dec bx
    cmp bx,0
    jg inner_loop
call newline
call down_triangle
dec ax
cmp ax, 0
jne outer_loop
call newline
call exit

exit:
  mov eax,1 ;sys_exit
  mov ebx,0     ;return 0
  int 80h;
  ret

newline:
  mov [char],byte 10
  push rax;
  push rbx;
  mov eax,4;    ;sys_write
  mov ebx,1;    ;stdout
  mov ecx, char;
  mov edx,1;    ;size of new line
  int 80h

  pop rbx;
  pop rax;
  ret

star:
  mov [char], byte '*';
  push rax;
  push rbx;
  mov eax,4;    ;sys_write
  mov ebx,1;    ;stdout
  mov ecx, char;
  mov edx,1;
  int 80h;
  pop rbx;
  pop rax;
  ret

down_triangle:
  push rax;
  push rbx;

  mov rax, [trianglerows]
  inc ax
  mov [trianglerows],rax

  pop rbx
  pop rax
  ret
```
I tried and tried and tried but I couldn't get what I needed to get.

I seem to be unable to find a way to separate the rows from the lines of stars, because of all those push and pop.

Honestly, I do not understand these much. I've been told that they are needed to execute the loops, but I am not sure why, for example, in the function star I would need to call the outer loop.

I couldn't find any combination of push and pop that worked. I am constantly getting either many stars or one star per line or just one star.

I am literally puzzled at which bits I'm changing and keeping the same. I was able to get the required output but one that never ended increasing.

I was able to get output starting from 10 stars and going down to one, but never what I wanted.

What am I doing wrong? How do I do this question?

# [Answer](https://stackoverflow.com/a/49346083/9210255)
I solved the problem this way, it's 32-bit:

```asm
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
shr eax,1 ; shr is used to divide by two
; now eax holds number of all symbols
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

;print ; edx has number of chars; ecx is pointer on the string
mov eax,4;  ;sys_write
inc ebx;    ;1 - stdout, at the end of the loop we have ebx=0
int 80h;

;exit
mov eax,1       ;1 -  sys_exit
xor ebx,ebx     ;0 - return 0
int 80h;
ret
```
How did I do it?
First of all, I count number of symbols what we have to print. I'll print it all at once. It's the sum of a finite arithmetic progression(arithmetic series).

TriangleImage

In our case we have

a1=2andd=1

Sn

We see 3 operations +, * and /. We can optimise only the division by 2, doing right shift:

```asm
lea eax, [ebx+3] ; n + 3
imul eax,ebx ; n * (n + 3)
shr eax,1 ; n * (n+3) / 2
```
Our string will be on the stack, let's prepare it to have enough memory:

```asm
mov ecx,esp
sub esp,edx
```
And then, we fill our stack by stars and \ns

```asm
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
```
I fill it backwards. What does it mean? I fill the string by symbols from the end to the beginning. Why do I do that? Just because I want to use less registers as it possible. At the end of the loop ecx contains a pointer on the string what we want to print. If I filled forwards, ecx contains a pointer on esp before "stack prepairing", and I can't use the register as a pointer on string in print function. Also I have to use another register to decrement or use cmp which is slower than dec.

That's all, print and end.

Another case

```asm
global _start

section .data
    rows dw 10

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
mov eax,4;  ;sys_write
mov ebx,1;  ;1 - stdout 
int 80h;

;exit
mov eax,1       ;1 -  sys_exit
xor ebx,ebx     ;0 - return 0
int 80h;
ret
```

Here, at the beginning we fill the stack by stars and only after that we fill it by \ns


