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
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0xd3
	msr cpsr, r0

#1. disable watchdog
	ldr r0, =0xE2700000
	mov r1, #0x0
	str r1, [r0]

#. led
	ldr r0, =0xE0200060
	ldr r1, =(0x11<<12)
	str r1, [r0]

	ldr r0, =0xE0200064
	ldr r1, =0x18
	str r1, [r0]

	b stop

# clock management unit
#ldr r0, =0xE0100200
#	and r1, r1, #0xfffffffe
#str r1, [r0]

#2. setup stack space
	ldr sp, =estack

#3. jump to main
	bl main

stop:
    b stop

bstack:	.space 32, 0
estack:	.space 1, 0






