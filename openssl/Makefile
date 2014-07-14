CC := gcc
CFLAGS := 

all: main base64

base64: base64.o

main: main.o aes_core.o
	$(CC) -o $@ $^

clean:
	rm main base64 *.o -rf

