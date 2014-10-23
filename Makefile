include ../Makefile_common

ifdef b
link_base	:= 0xd0020000		# iRAM running address
else
link_base	:= 0x40008000		# ddr running address
endif

elf_file	:= units.elf
bin_file	:= units.bin
dis_file	:= units.dis

objs	:= start.o
objs	+= units.o
objs	+= uart.o
objs	+= common.o

$(bin_file): $(objs)
	$(LD) -Ttext $(link_base) $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)
	make install

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.dis *.out test

install:
	cp $(bin_file) $(tftp_root)

