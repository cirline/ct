include ../Makefile_common

ifdef link
link_base	:= text 0xd0020000		# iRAM running address
else
#link_base	:= text 0x40008000		# ddr running address
#link_base	:= text 0x23e00000
link_base	:= map.lds
endif

elf_file	:= main.elf
bin_file	:= main.bin
dis_file	:= main.asm

objs	:= start.o
objs	+= main.o
objs	+= uart.o
objs	+= common.o
objs	+= irq.o
objs	+= nf.o
objs	+= timer.o
objs	+= rtc.o
objs	+= i2c.o
objs	+= lcd.o
objs	+= keyboard.o

$(bin_file): $(objs)
	$(LD) -T$(link_base) $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)
	make install

test: test.o

clean:
	rm -rf *.o *.elf *.bin *.asm *.out *.i test *.dis

install:
	cp $(bin_file) $(tftp_root)

