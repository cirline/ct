objs = main.o
objs += dir1/sub1.o

CC	:= gcc

main.out:$(objs)
	$(CC) -o $@ $^

clean:
	rm -rf *.o *.out
