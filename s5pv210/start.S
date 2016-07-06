/**
 * Copyright (c) 2015 Qiwei Chen
 *
 ************************
 * !!! Some WARNING !!! *
 ************************
 * 1. if use IRQ, don't forget relocate IRQ vendor. see WARNING1
 * 2. select correct DRAM link address for different boot mode, according to warning1
 * 3. if use nand boot, start.S must set the 16 Bytes file header, see WARNING2 
 */

/* !!! WARNING2 !!! */
# s5pv210 file header, total 16 bytes
# iROM jump to iRAM(0xD002_0010) but iRAM is begin 0xD002_0000
#.word 0x2000
#.word 0x0
#.word 0x0
#.word 0x0

.text
.global _start
_start:
# reset
	b	reset
# undefined
	b	reset
# soft interrupt
	b	reset
# pre fetch instruction stop
	b	reset
# data stop
	b	reset
# remain
	b	reset
# IRQ interrupt
	b	irq_request
# FIQ interrupt
	b	reset

reset:
# svc mode
	msr cpsr_c, #0xd3

# disable watchdog
	ldr r0, =0xE2700000
	mov r1, #0x0
	str r1, [r0]

/** !!! WARNING1 !!!
 * relocate interrupt vendor
 * mode:
 tftp:  0 -> 40008000
 usb:   0 -> 23e00000
 nand:  0 -> d0020000 ?? 16byte ?
 */
	ldr r0, =0x40008000
	mcr p15, 0, r0, c12, c0, 0

led_test:
    mov r0, #0
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

/********************
 * memory map       *
 ********************
 * .text
 * .rodata
 * .bss
 * .data
 * sys stack
 * irq stack
 * svc stack
 */

# setup stack space
set_stack:
#	mov r0, pc
#	sub r0, r0, #8			@# r0 = set_stack = currect address
#	ldr r1, =set_stack		@# r1 = relative set_stack
#	ldr r2, =stack_top		@# r2 = relative stack_top
#	sub r1, r2, r1			@# r1 = address gap
#	add r0, r0, r1			@# r0 = absolute stack_top
	ldr r0, =(__data_end + 0x3000)
# svc mode
	msr cpsr_c, #0xd3
	mov sp, r0
	sub r0, r0, #0x1000
# irq mode
	msr cpsr_c, #0xd2
	mov sp, r0
	sub r0, r0, #0x1000
# sys mode, and enable irq
	msr cpsr_c, #0xdf
	mov sp, r0
	msr cpsr_c, #0x5f

# clean bss section
	ldr r0, =__bss_start
	ldr r1, =__bss_end
	mov r2, #0
bss_clean:
	cmp r0, r1
	strne r2, [r0]
	addne r0, r0, #4
	bne bss_clean

# jump to main
	bl main

stop:
	b stop

/**
 * if r0 == 0, light on the led
 * else, program will stay here to flash the led
 */
led_debug:
# set GPA0CON output
	ldr r1, =0xE0200060
	ldr r2, =(0x11<<12)
	str r2, [r1]
# GPA0DAT register
	ldr r1, =0xE0200064
led_death_loop:
# delay
	ldr r2, =0x8fffff
ld_loop2:
	sub r2, r2, #1
	cmp r2, #0
	bgt ld_loop2
# light on
	mov r2, #0x8
	str r2, [r1]

// if r0 == 0, return
    cmp r0, #0
    moveq pc, lr

	ldr r2, =0x8fffff
ld_loop3:
	sub r2, r2, #1
	cmp r2, #0
	bgt ld_loop3

# light off
	mov r2, #0
	str r2, [r1]
	b led_death_loop

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

irq_request:
	stmfd sp!, {r0-r12, lr}
	bl	irq_handler
	ldmfd sp!, {r0-r12, pc}^
	subs pc, lr, #4


