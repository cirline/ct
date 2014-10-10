include ../Makefile_common

elf_file	:= units.elf
bin_file	:= units.bin
dis_file	:= units.dis

objs	:= start.o
objs	+= units.o

$(bin_file): $(objs)
	$(LD) -Ttext 0x40008000 $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.dis test

