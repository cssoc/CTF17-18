SECTION .rodata
	greeting: db "ECHO SERVICE: NOW IN ASM", 0xa, "q to quit", 0xa, 0x0
	greet_len equ $-greeting
	quit: db "q", 0xa, 0x0
	db "/bin/sh", 0x0

SECTION .text
	GLOBAL _start

read:
	mov rax, 0
	syscall
	ret
write:
	mov rax, 1
	syscall
	ret

exit:
	mov rax, 60
	syscall
	ret

_start:
	call main

	mov rdi, 0
	call exit

strcmp:
	mov al, byte[rdi]
	sub al, byte[rsi]
	jne .exit
	cmp byte[rdi], 0
	je .exit
	inc rdi
	inc rsi
	jmp strcmp
.exit:
	ret

main:
	push rbp
	mov rbp, rsp

	sub rsp, 0x100

	mov rdi, 1
	mov rsi, greeting
	mov rdx, greet_len
	call write

.loop:	mov rdi, 0
	lea rsi, [rbp - 0xf0]
	mov rdx, 0x300
	call read

	mov byte[rbp - 0xf0 + rax], 0
	mov qword[rbp - 0x100], rax

	lea rdi, [rbp - 0xf0]
	mov rsi, quit
	call strcmp
	mov qword[rbp - 0xf8], rax

	mov rdi, 1
	lea rsi, [rbp - 0xf0]
	mov rdx, qword[rbp - 0x100]
	call write

	cmp qword[rbp - 0x100], 0
	je .exit
	cmp qword[rbp - 0xf8], 0
	jne .loop

.exit	leave
	ret
