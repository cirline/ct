include Makefile_common

CFLAGS	:= -Wall
CFLAGS	+= -I. -Iinclude

ifdef link
link_base	:= text 0xd0020000		# iRAM running address
else

# if change lds, must care irq vendor in start.S

#link_base	:= text 0x40008000		# ddr running address
#link_base	:= map_usb_boot.lds
link_base	:= map_tftp_boot.lds
#link_base	:= map.lds
endif

elf_file	:= main.elf
bin_file	:= main.bin
dis_file	:= main.dis

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
objs	+= task.o

objs	+= shell/shell.o

$(bin_file): $(objs)
	$(LD) -T$(link_base) $(objs) -o $(elf_file)
	$(OBJCOPY) -O binary -S $(elf_file) $@
	$(OBJDUMP) -D -m $(arch) $(elf_file) > $(dis_file)
	make install

test:
	gcc -o regaddr_test.out regaddr_test.c

clean:
	rm -rf *.o *.elf *.bin *.asm *.out *.i test *.dis

install:
	cp $(bin_file) $(tftp_root)/units.bin

tag:
	ctags -R .

.PHONY: tag

