include ../Makefile_common

elf_file	:= led.elf
bin_file	:= led.bin
dis_file	:= led.dis

objs	:= start.o
objs	+= led.o

$(bin_file): $(objs)
	$(LD) -Ttext 0x40008000 $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.dis test

