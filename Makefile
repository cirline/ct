include ../Makefile_common

link_base	:= 0x00000000

elf_file	:= units.elf
bin_file	:= units.bin
dis_file	:= units.dis

objs	:= start.o
objs	+= units.o

$(bin_file): $(objs)
	$(LD) -Ttext $(link_base) $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)
	make install

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.dis test

install:
	cp $(bin_file) $(tftp_root)

