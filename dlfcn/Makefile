CC := gcc
CFLAGS += -ldl

main: main.o dlfcn.so
	gcc -o $@ $< -ldl -rdynamic

dlfcn.so: dlfcn.c
	gcc -shared -fPIC -nostartfiles -o $@ $<

clean:
	rm -r *.so *.o main
