
target_file	:= modtaxi
test_file	:= uart

CC		:= gcc
CFLAGS	:= -fPIC -L. -l$(target_file)

objs		:= modtaxi.o

all: $(target_file) $(test_file)
	make install

$(target_file): $(objs)
	$(CC) -o lib$@.so -shared $^

$(test_file): $(test_file).o
	$(CC) -o $@.out $^ $(CFLAGS)

install:
	sudo cp lib$(target_file).so /lib/

clean:
	rm -rf *.o *.out *.so

