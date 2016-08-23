CC := gcc
LIBS += -lpthread -ldl
CFLAGS += -Wall

SOFT := mysh
#OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJS := main.o
OBJS += shell.o
OBJS += field.o

$(SOFT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o $(SOFT) *.exe
