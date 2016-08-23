.section .data
.global data_start
data_start:
.long 3,67,28
.section .text
.global _start
_start:
	mov $1, %eax
	mov $4, %ebx
	int $0x80
