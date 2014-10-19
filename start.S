# s5pv210 file header, total 16 bytes
# iROM jump to iRAM(0xD002_0010) but iRAM is begin 0xD002_0000
.word 0x2000
.word 0x0
.word 0x0
.word 0x0

.text
.global _start
_start:
	b	reset
reset:
# SVC mode
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0xd3
	msr cpsr, r0

# disable watchdog
	ldr r0, =0xE2700000
	mov r1, #0x0
	str r1, [r0]

# led test
	ldr r0, =0xE0200060
	ldr r1, =(0x11<<12)
	str r1, [r0]
	mov r0, #1
	bl led_debug

# close mmu and data cache
#	L1 I/D
	mov	r0, #0
	mcr	p15, 0, r0, c8, c7, 0
	mcr	p15, 0, r0, c7, c5, 0
#	MMU stuff and cache
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000
	bic	r0, r0, #0x00000007
	orr	r0, r0, #0x00000002
	orr	r0, r0, #0x00000800
	mcr	p15, 0, r0, c1, c0, 0

# clock management unit
	bl clock_init

# setup stack space
set_stack:
	sub r0, pc, #8
	ldr r1, =set_stack
	ldr r2, =end_stack
	sub r1, r2, r1
	add sp, r0, r1

# jump to main
	bl main

stop:
    b stop

led_debug:
	ldr r1, =0xE0200064
ld_loop1:
	cmp r0, #0
	beq ld_end

	mov r2, #0x10
	str r2, [r1]

	ldr r2, =0x8fffff
ld_loop2:
	sub r2, r2, #1
	cmp r2, #0
	bgt ld_loop2

	mov r2, #0x8
	str r2, [r1]

	ldr r2, =0x8fffff
ld_loop3:
	sub r2, r2, #1
	cmp r2, #0
	bgt ld_loop3

	sub r0, r0, #0x1
	b ld_loop1
ld_end:
	mov pc, lr

clock_init:
#	reset clock src mux	
	ldr r0, =0xE0100200
	mov r1, #0
	str r1, [r0]
#	set pll
	ldr r0, =0xE0100100
	ldr r1, =(1<<31 | 125<<16 | 3<<8 | 1<<0)
	str r1, [r0]
	ldr r0, =0xE0100108
	ldr r1, =(0x1<<31 | 667<<16 | 12<<8 | 1<<0)
	str r1, [r0]
	ldr r0, =0xE0100110
	ldr r1, =(0x1<<31 | 48<<16 | 3<<8 | 2<<0 | 1<<26)
	str r1, [r0]
	ldr r0, =0xE0100120
	ldr r1, =(0x1<<31 | 108<<16 | 6<<8 | 3<<0)
	str r1, [r0]
# wait to lock
	mov r1, #0x10000
wait_lock:
	sub r1, #1
	cmp r1, #0
	bne wait_lock
# select clock
	ldr r0, =0xE0100200
	ldr r1, [r0]
	ldr r2, =(0x1111)
	orr r1, r2
	str r1, [r0]
# set divider
	ldr r0, =0xE0100300
#ldr r1, =(3<<0 | 3<<4 | 3<<8 | 3<<12 | 3<<16 | 3<<20 | 3<<24 | 3<<28)
	mov r1, #0
#str r1, [r0]

	mov pc, lr



fstack:	.space 128, 0
end_stack:





