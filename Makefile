include ../Makefile_common

ifdef link
link_base	:= text 0xd0020000		# iRAM running address
else
#link_base	:= text 0x40008000		# ddr running address
#link_base	:= text 0x23e00000
link_base	:= map.lds
endif

elf_file	:= units.elf
bin_file	:= units.bin
dis_file	:= units.asm

objs	:= start.o
objs	+= units.o
objs	+= uart.o
objs	+= common.o
objs	+= irq.o
objs	+= nf.o

$(bin_file): $(objs)
	$(LD) -T$(link_base) $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)
	make install

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.dis *.out test

install:
	cp $(bin_file) $(tftp_root)

