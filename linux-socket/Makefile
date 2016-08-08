CC	:= cc
#CFLAGS	+= -Wall

sobjs	:= ls_server.o

cobjs	:= ls_client.o

all:ls_server.elf ls_client.elf

ls_server.elf:$(sobjs)
	$(CC) -o $@ $< $(CFLAGS)

ls_client.elf:$(cobjs)
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o *.elf


