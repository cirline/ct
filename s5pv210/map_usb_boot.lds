OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x23e00000;
	. = ALIGN(4);
	.text : {
		*(.text)
	}
	.rodata : {
		*(.rodata)
	}
	__bss_start = .;
	.bss : {
		*(.bss)
	}
	__bss_end = .;
	__data_start = .;
	.data : {
		*(.data)
	}
	__data_end = .;
}
