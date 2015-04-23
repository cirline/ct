CC := gcc
CFLAGS := 

all: main base64

base64: base64.o

main: main.o aes_core.o cbc128.o aes_cbc.o
	$(CC) -o $@ $^

clean:
	rm main base64 *.o -rf

