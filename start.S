.text
.global _start
_start:
#1. disable watchdog
	ldr r0, =0xE2700000
	ldr r1, =0x0
	str r1, [r0]

# clock management unit
	ldr r0, =0xE0100200
	and r1, r1, #0xfffffffe
#str r1, [r0]

#2. setup stack space
	ldr sp, =0x40008000

#3. jump to main
	bl main

stop:
    b stop





