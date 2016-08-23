CC	:= gcc
CFLAGS	+=

all:ls_server.out ls_client.out


ls_server.out:ls_server.o
	$(CC) -o $@ $^ $(CFLAGS)

ls_client.out:ls_client.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o *.out

